extends MeshInstance3D

@export var size := 100
@export var step := 1.0

func _ready():
	var generated_mesh := ImmediateMesh.new()
	generated_mesh.clear()
	generated_mesh.surface_begin(Mesh.PRIMITIVE_LINES)

	for i in range(-size, size + 1):
		var color = Color(0.3, 0.3, 0.3)
		if i % 10 == 0:
			color = Color(0.1, 0.1, 0.1)

		# linie wzdłuż X
		generated_mesh.surface_add_color(color)
		generated_mesh.surface_add_vertex(Vector3(i * step, 0, -size * step))
		generated_mesh.surface_add_color(color)
		generated_mesh.surface_add_vertex(Vector3(i * step, 0, size * step))

		# linie wzdłuż Z
		generated_mesh.surface_add_color(color)
		generated_mesh.surface_add_vertex(Vector3(-size * step, 0, i * step))
		generated_mesh.surface_add_color(color)
		generated_mesh.surface_add_vertex(Vector3(size * step, 0, i * step))

	generated_mesh.surface_end()
	self.mesh = generated_mesh
