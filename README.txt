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
- All except Camera movement part

Which part of the assignment has NOT been completed (not done, not working, etc.) and explanation on why those parts are not completed?
- Camera movement part is not done. ImGui Input is not working for me.

Where the relevant source codes (both C++ and shaders) for the assignment are located. Specify the file path (folder name), file name, function name (or line number).
- Scene generation 
	- Folder name: Project
	- File name: TestScene2.cpp (line 47)
- Deferred Shading
	- Folder name: Project
	- File name: TestScene2.cpp (line 81, 527)
	- Folder name: Shaders
	- File name: F_GeometryStageShader.vert, .frag, F_LightingShaderForLight.vert, .frag
- Forward Shading
	- Folder name: Project
	- File name: TestScene2.cpp (line 577)
- Interactivity
	- Folder name: Project
	- File name: TestScene2.cpp (line 372) 

Which machine did you test your application on.
- Indivisual laptop. 

If remote – specify OS, GPU, and OpenGL Driver version. 
- Windows 10, GTX 1660 Ti, 4.6

The average number of hours you spent on the assignment, on a weekly basis
- about 4 hours per a week.

Any other useful information pertaining to the application 
- Nothing