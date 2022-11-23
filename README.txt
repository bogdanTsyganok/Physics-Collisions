author: Bogdan Tsyganok, 0886354
video link: https://youtu.be/siFK6iY8wA8
Please reach out if there are any problems with the video.

Description:
A collision mash-up with extendable configuration
WASDQE to move camera
UO to "look" up/down
1 to first shape86
2 to second shape
3 to third shape
R to render cylinders (mesh is fixed but orientation is still wrong)
left and right to select spaned object
NUMPAD 456879 to "push" selected object around


Running:
Open the .sln file, run in Debug or Release and x64 configuration


Relevant classes/lines:
		ParticleContact
		ParticleWorld
		ParticleContactGenerator
		ParticleContactResolver
		ParticleConstraints
		helpers.h
		PinballObjectBuilder
		PinballToy
		ObjectLoader
		ArenaLoader

CreateProjectile at the end of main calls builder methods to create and apply init force


EndDocument



