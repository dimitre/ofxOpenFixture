# ofxOpenFixture

Openframeworks addon to handle DMX Fixtures using Open Fixture Library. 
https://github.com/OpenLightingProject/open-fixture-library
https://open-fixture-library.org/
Development started in June 2017 by @Hperigo

Example uses ofxArtnet (Legacy branch), it will be updated to actual library soon.
https://github.com/hiroyuki/ofxArtnet/

And second example uses ofxDmtrUI3
https://github.com/dimitre/ofxDmtrUI3/

It can use OFL fixture files in JSON format, or accept its own format similar to configparser.

```
[laser]
1 = mode
2 = color
3 = shape
4 = offY
5 = offX
6 = scaleY
7 = scaleX
8 = rotate
9 = estraga
10 = scale
11 = dots

ui = smallnewcol
audio = 2, 3, 4, 9, 11, 12
beat = 2, 3, 4, 9, 11, 12
flip = 7
```
