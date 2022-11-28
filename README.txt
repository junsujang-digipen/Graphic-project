Complete name of the student (first name and last name) 
- Junsu Jang
DigiPen Id 
- junsu.jang

How to use parts of your user interface that is NOT specified in the assignment description.
- Collapsed headers include the topics control
- In Obj control header, there are reflect and refract controls exist
- Slider 'Normal': 0 means not show normal vector, 1 means show vertex normal vector, 2 means show face normal vector, 3 means show both normal vector.
- Slider 'OBJ Number': 0 means show 4Sphere, 1 means show bunny_high_poly, 2 means show cube2, 3 means show sphere, 4 means show sphere_modified
- Slider 'OBJ rotate': means rotating loaded obj
- Slider 'Orbit speed': means change speed of rotating spheres
- Slider 'Attenuation coefficients': means change c1, c2, c3 for computing attenuation
- Light control will be changed according to selected scenario

Any assumption that you make on how to use the application that, if violated, might cause the application to fail.
- Make sure build the two projects
- Set the "Project" project to start project

Which part of the assignment has been completed?
- All

Which part of the assignment has NOT been completed (not done, not working, etc.) and explanation on why those parts are not completed?
- Nothing

Where the relevant source codes (both C++ and shaders) for the assignment are located. Specify the file path (folder name), file name, function name (or line number).
- Sky box rendering
	- Folder name: GAPP
	- File name: SkyBox.h, .cpp
	- Folder name: Shaders
	- File name: FF_CommonFunctions.glsl (line 91), F_SkyBoxShader.frag
- Scene setup & Environment Map generation
	- Folder name: GAPP
	- File name: TestScene2.cpp (line 107,497)
	- File name: RenderTextureCamera.h, .cpp
- Environment Map application in shaders
	- Folder name: Shaders
	- File name: F_PhongShadingReflectShader.frag (line 151)
- User Interactivity – Rendering
	- Folder name: GAPP
	- File name: TestScene2.cpp (line 366) 

Which machine did you test your application on.
- Indivisual laptop. 

If remote – specify OS, GPU, and OpenGL Driver version. 
- Windows 10, GTX 1660 Ti, 4.6

The average number of hours you spent on the assignment, on a weekly basis
- about 8 hours per a week.

Any other useful information pertaining to the application 
- Nothing