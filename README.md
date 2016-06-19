# 给rpgmaker的地图生成光影

原理见：[让地图加分的方法！室内地图光影+无限图层](http://rm.66rpg.com/thread-388527-1-1.html)

其中，无限图层脚本见：[ULDS by taroxd](http://rm.66rpg.com/thread-367901-1-1.html)

生成效果并不是很好，因为程序无法从二维图片中提取xyz三个坐标，只能完全当二维来处理。

## 效果

![原图](https://raw.githubusercontent.com/garfeng/shadow/master/examples/originMap.png) ![光+影](https://raw.githubusercontent.com/garfeng/shadow/master/examples/shadowAndLight.png) ![影](https://raw.githubusercontent.com/garfeng/shadow/master/examples/onlyWithShadow.png) ![光](https://raw.githubusercontent.com/garfeng/shadow/master/examples/onlyWithLight.png)


## 可运行程序下载

[百度网盘](http://pan.baidu.com/s/1i5deUwP)

## 使用方法

[帮助文档](http://garfeng.github.io/2016-06-19/Auto-Shadow/)

## 编译

### 界面部分

用QtCreator打开shadow.pro，然后编译就好了。

### 核心算法

核心逻辑用的是golang写的，但您多半编译不了，涉及到各种小的包文件，有很多自己写的还没上传，而且也不打算上传（都是些几行，几十行的用来偷懒的不稳定的东西）。

而且，坚决不推荐你来看算法的代码，它一定会confusing you，还是推荐看原理：[让地图加分的方法！室内地图光影+无限图层](http://rm.66rpg.com/thread-388527-1-1.html)
