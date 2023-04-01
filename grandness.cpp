//
// grandness - quantify the "grandness" of a mountain/terrain
//
// (c)2021 Mark J. Stock <markjstock@gmail.com>
//

#include "inout.h"
#include "CLI11.hpp"

#include <Eigen/Core>

#include <cassert>
#include <vector>
#include <limits>
#include <cmath>
#include <iostream>



// begin execution here

int main(int argc, char const *argv[]) {

  std::cout << "# grandness v0.1\n";

  // process command line args
  CLI::App app{"Measure the grandness of a mountain from a DEM"};

  // load a dem from a png file - check command line for file name
  std::string demfile;
  app.add_option("-d,--dem", demfile, "digital elevation model as png");

  // the vertical scale of black-to-white, scaled by the x size of the terrain
  float vscale = 0.1;
  app.add_option("-v,--vert", vscale, "vertical scale of dem w.r.t. horizontal scale");

  // write out certain arrays
  std::string outmap;
  app.add_option("--om", outmap, "write slope map to a png");

  // finally parse
  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError &e) {
    return app.exit(e);
  }


  //
  // Preliminary work - load in the arrays
  //

  // load the data

  // array of the ground elevations (DTM)
  Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> elev;

  size_t nx = 0;
  size_t ny = 0;

  if (not demfile.empty()) {
    // read a png to get the elevation
    // check the resolution first
    int hgt, wdt;
    (void) read_png_res (demfile.c_str(), &hgt, &wdt);
    if (wdt > 0) nx = wdt;
    if (hgt > 0) ny = hgt;

    // allocate the space
    elev.resize(nx,ny);
    float** data = allocate_2d_array_f((int)nx, (int)ny);

    // read the first channel into the elevation array, scaled as 0..vscale
    (void) read_png (demfile.c_str(), (int)nx, (int)ny, 0, 0, 0.0, 0,
                     data, 0.0, vscale*(float)nx, nullptr, 0.0, 1.0, nullptr, 0.0, 1.0);

    for (size_t i=0; i<nx; ++i) for (size_t j=0; j<ny; ++j) elev(i,j) = data[i][j];

    free_2d_array_f(data);
  }

  // define the slope bins
  const size_t nbins = 100;
  const float lolog = -3.0;
  const float hilog = 2.0;
  Eigen::ArrayXi bin(nbins);
  bin = 0;


  //
  // Process each possible pair of pixels once
  //
  const size_t ntot = nx*ny;
  float maxslope = 0.0;

  // loop over all pixel pairs "source" and "target"
  #pragma omp parallel for schedule(guided)
  for (size_t is=0; is<ntot; ++is) {
    const size_t isx = is / nx;
    const size_t isy = is % nx;

    for (size_t it=is+1; it<ntot; ++it) {
      const size_t itx = it / nx;
      const size_t ity = it % nx;

      // find elevation difference and logarithmic bin
      const float delev = elev(isx,isy) - elev(itx,ity);
      // and slope
      const float dist = std::sqrt(std::pow(itx-isx,2) + std::pow(ity-isy,2));
      // convert to a bin
      const int32_t ibin = nbins * (std::log10(delev/dist) - lolog) / (hilog-lolog);

      //if (delev/dist > maxslope) {
      //  std::cerr << "slope " << (delev/dist) << " from elevs " << elev(isx,isy) << "," << elev(itx,ity) << " between " << isx << "," << isy << " and " << itx << "," << ity << " (delev " << delev << " dist " << dist << ")\n";
      //  maxslope = delev/dist;
      //}

      // increment that counter
      if (ibin > -1 and ibin < nbins) {
        #pragma omp atomic
        bin[ibin] += 1;
      }
    }

  } // end loop over all start points

  //
  // Wrapping up - write image output
  //

  for (size_t i=0; i<nbins; ++i) {
    std::cout << std::pow(10.0,(hilog-lolog)*(i+0.5)/nbins + lolog) << " " << (bin[i]/std::pow((float)(nx*ny),2)) << "\n";
  }

  // write out a combination of the dem and path matrix as a png
  /*
  if (not outmap.empty()) {
    std::cout << "\nWriting map to " << outmap << std::endl;

    float** data = allocate_2d_array_f((int)nx, (int)ny);
    for (size_t i=0; i<nx; ++i) for (size_t j=0; j<ny; ++j) data[i][j] = elev(i,j);

    // make the line darker or lighter (to see it)
    const float factor = 1.0/(vscale*(float)nx);
    for (size_t i=0; i<nx; ++i) for (size_t j=0; j<ny; ++j) {
      // add easy and path together
      const float pathsum = std::max(pathimg(i,j), easy(i,j));
      if (data[i][j]*factor < 0.5) {
        data[i][j] += 0.5*pathsum/(path_max*factor);
      } else {
        data[i][j] -= 0.5*pathsum/(path_max*factor);
      }
    }

    (void) write_png (outmap.c_str(), (int)nx, (int)ny, FALSE, TRUE,
                      data, 0.0, vscale*(float)nx, nullptr, 0.0, 1.0, nullptr, 0.0, 1.0);

    free_2d_array_f(data);
  }
  */
}
