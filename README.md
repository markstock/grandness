# grandness
An integrative measure of the magnitude of a mountain

## Setup
Compile and run with:

    make
    ./grandness -h
    ./grandness -d data/aconcagua.png -v 0.26016

Use `-d` to set the DEM (digital elevation model) and `-v` to set the slopes - the vertical distance between pure black and white divided by the horizontal (left-to-right) length of the represented area. For example, if your png were 1000 by 500 pixels and each pixel is 100 meters, then the width of your image is 100,000 meters. If black is -100m and white is 4900m, then the vertical range is 5000 meters. The coefficient for this case would be 0.05.

## Background
Since the beginning of measurements humans have measured mountains, but typically only by the elevation of their summits, and then typically only as a distance above mean sea level. Eventually we began using a measurement called "prominence", which is the elevation of a summit above the highest col (saddle) between it and the next higher summit. Read more about this measurement and it's drawbacks [on wikipedia](https://en.wikipedia.org/wiki/Topographic_prominence).

As subjective measures of the greatness of a mountain, both of these common measurements are intrinsically flawed, in that they deal only with the summit elevation itself, and not of the qualities that makes a mountain "grand" (for lack of a better term). This code seeks to explore and possibly establish a new quantitative measurement that more closely tracks with people's perceptions of what makes a mountain great and worth visiting or climbing.

## To Do
* Add options for meters per pixel, meters black-to-white
* Test same DEMs at different resolutions - want to have it return similar results
* Test same DEMs cropped closer or looser - again want similar results
* Test vs. same area just half as tall
* Test vs. 2x area at same height
* Test weird places like Grand Canyon, Shiprock, Black Canyon, Yosemite
* Write something up

## Credits
This code was written by Mark Stock <markjstock@gmail.com> in 2023. [CLI11](https://github.com/CLIUtils/CLI11) is by Henry Schreiner.

