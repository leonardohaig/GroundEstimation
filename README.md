## 说明 ##
1.代码名称：GroundEstimation
2.代码作用：利用双目相机，提取道路区域
3.OpenCV版本：OpenCV3.1.0 or later。无需opencv_contrib模块
4.代码运行说明：
1）读取双目相机图片，采用的是kitti数据集；
2）采用StereoSGBM算法计算得到视差图，耗时约60ms左右；
3）根据视差图提取路面：
1°从视差图得到U-视差图--computeUDisparity()；
2°从视差图中将障碍物区域提取出来--removeObstacle();
> Note：1°和2°可以并行计算；

3°从视差图和2°中的障碍物区域提取得到V-视差图--computeVDisparity()；
4°利用霍夫直线检测函数对V-视图进行检测，判断并提取其中的地面线段--groundLineFromVDisp();
5°利用视差图和4°中的地面线段初步得到路面区域--groundMapExtraction();
6°利用视差图和5°中得到的路面区域，进行RANSAC拟合，得到路面--groundPlaneEstimation();
> 若5°中没有得到路面区域，则该步仅利用视差图进行RANSAC平面拟合，进行路面提取；

7°新建图像，将路面区域置黑，其他区域置白（255）；
8°利用原图及7°中的二值化图，在原图中以指定颜色显示路面，便于查看效果。