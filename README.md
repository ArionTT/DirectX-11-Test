# DirectX-11-Test
This is a test projection about DirectX 11. 
   
##Description
This project is written by C++ Language and DirectX 11 Graphic Lib.Inside,mesh is readed and translated by a C++ lib, tiny_obj_reader.
   
##Files Structure
Application class ----it is a class which control WinAPI and Device class.   
Device class---------it contains D3D Object and various of Devcie ,for example Camera,Mesh,Timer,etc.   
Camera class--------it contains Camera Controller and Input class.   
Input class----------it contains DirectInput Device .    
Mesh2 class-------- it uses inherit tiny_obj_reader lib to get vertex and index data of Model.    
Model class---------it contains Mesh2 data and translate it to VertexType array data or WORD array data.   
pixel.fx--------------| there are shader files for Model class.   
vert.fx    
gpixel.fx------------|there are shader files for Grids Class.   
gvert.fx   
Path/model/-------|this is a path store model obj data.    

##Image
![image](https://github.com/ArionTT/DirectX-11-Test/blob/master/AppImg/screenshot.png)
