# mini3d
3D软件渲染教程，并没有任何性能优化，主要向人说明如何写一个固定管线的软件渲染器。虽然主体代码只有 700行，但是麻雀虽小，五脏俱全。

特性
===
* 单文件源代码：源代码只有一个 mini3d.c，包含所有内容。
* 数学库：基础矢量和矩阵数学计算
* 坐标变换：齐次坐标计算，变换模型仿照 D3D的左手系，及 WORLD/VIEW/PROJ 三矩阵
* 纹理支持：最大支持 1024 x 1024 的纹理
* 透视贴图：透视纹理映射以及透视色彩插值
* 实现精简：渲染引擎只有 700行。
* 详细注释：主要代码详细注释

编译
===
* mingw: 
		gcc -O3 mini3d.c -o mini3d.exe
* msvc:
		cl -O2 -nologo mini3d.c -lgdi32

演示
===

色彩填充：RENDER_STATE_COLOR
![](https://raw.githubusercontent.com/skywind3000/mini3d/master/images/mini_0.png)

纹理填充：RENDER_STATE_TEXTURE
![](https://raw.githubusercontent.com/skywind3000/mini3d/master/images/mini_1.png)

线框绘制：RENDER_STATE_WIREFRAME
![](https://raw.githubusercontent.com/skywind3000/mini3d/master/images/mini_2.png)


基础作业
=======
* 增加背面剔除
* 增加简单光照
* 实现二次线性差值的纹理读取
* 针对：所有人

进阶作业
=======
* 提供更多渲染模式
* 优化顶点计算性能
* 优化 draw_scanline 性能
* 定点数实现优化计算 
* 从 BMP/TGA 文件加载纹理
* 载入 BSP 场景并实现漫游
* 针对：感兴趣的人，想继续优化的人


