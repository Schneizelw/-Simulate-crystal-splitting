# -Simulate-crystal-splitting
 基于晶格形式不规则体二分分裂算法的实现
 
## 问题描述：
 
  存在一个不规则体，且该物体会不断的分裂，分裂的位置随机，并且分裂一直进行到全部为单体(不可再分)时才停止。
  
  给出物体在三维空间中的位置，在file文件中每个点x,y,z给出。按照要求模拟物体分裂：
  
  在二维平面的分裂要求：
  
 
  ![Image not found](https://github.com/Schneizelw/-Simulate-crystal-splitting/blob/master/img/002.png)
  
  ![Image not found](https://github.com/Schneizelw/-Simulate-crystal-splitting/blob/master/img/003.png)
  
  ![Image not found](https://github.com/Schneizelw/-Simulate-crystal-splitting/blob/master/img/004.png)
  
  note0：该物体用一个三维数组表示。
  
  note1：分裂的方向随机，x,y,z轴都可以，且分裂位置随机，该那一边移动也是随机。
  
  note2：当一个体系中有多个非单体团簇的时候，随机一个团簇分裂。
  
  note3：当分裂的时候，如果旁边是其他团簇则将其往同一个方向移动，也就是"挤过去"。
  
  note4：如果一直挤到了边缘则需要回溯，从其他方向切割。
  
  note5：每一步的分裂都要生成txt文件将其每个点的三维坐标保存下来。最后用matlab显示结果。
  
  如部分切割结果：
  
  <div align=center><img width="555" height="425" src="https://github.com/Schneizelw/-Simulate-crystal-splitting/blob/master/img/005.png"/></div>
  
   <div align=center><img width="555" height="425" src="https://github.com/Schneizelw/-Simulate-crystal-splitting/blob/master/img/006.png"/></div>
   
   <div align=center><img width="555" height="425" src="https://github.com/Schneizelw/-Simulate-crystal-splitting/blob/master/img/007.jpg"/></div>
  
