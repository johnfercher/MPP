Multiple Path Planning
----------------------

[![MIT licensed](https://img.shields.io/badge/license-MIT-blue.svg)][mit]


Install (Debian 8)
------------------
**MPP Install**
```
$ sudo apt-get update && apt-get upgrade
$ sudo apt-get install build-essential pkg-config git cmake
$ sudo apt-get install gnuplot libqt4-core libqt4-dev protobuf-compiler libprotobuf-dev freeglut3 freeglut3-dev libompl-dev libboost-all-dev
```

**grSim Install** [original][grsim-install]
```
$ sudo apt-get install libode1 libode-dev

$ wget http://vartypes.googlecode.com/files/vartypes-0.7.tar.gz
$ tar xfz vartypes-0.7.tar.gz
$ cd vartypes-0.7
$ mkdir build && cd build
$ cmake ..
$ make 
$ sudo make install

$ git clone https://github.com/mani-monaj/grSim.git
$ cd grSim
$ make
```

Install (Ubuntu 14.04)
----------------------
```
$ TODO
```


License
-------

This code is licensed under the [MIT LICENSE][mit], of which a textual copy is available at [LICENSE.txt][license].

[mit]: https://raw.githubusercontent.com/hyperium/hyper/master/LICENSE
[protobuf]: https://developers.google.com/protocol-buffers/
[travis]: https://travis-ci.org/SIRLab/VSS-Vision
[ompl]: http://ompl.kavrakilab.org/
[gnuplot]:http://www.gnuplot.info/
[glut]: http://freeglut.sourceforge.net/
[ssl-vision]: https://github.com/RoboCup-SSL/ssl-vision
[grsim]: https://github.com/mani-monaj/grSim
[license]: https://github.com/johnfercher/MPP/blob/master/LICENSE.txt
[grsim-install]: https://github.com/mani-monaj/grSim/blob/master/INSTALL.md