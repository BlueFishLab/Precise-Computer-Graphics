extends Node2D

@onready var _lines: Node2D = $Line2D
@onready var _button_panel: GridContainer = $CanvasLayer/ScrollContainer/ColorButtonsPanel

var _pressed: bool = false
var _current_line: Line2D = null
var _line_color: Color = Color.BLUE

var _colors := [
	Color.RED, Color.GREEN, Color.BLUE, Color.YELLOW, Color.ORANGE, Color.PURPLE,
	Color.CYAN, Color.MAGENTA, Color.BROWN, Color.GRAY, Color.DARK_GREEN, Color.DARK_RED,
	Color.SKY_BLUE, Color.SEA_GREEN, Color.GOLD, Color.LIGHT_CORAL, Color.NAVY_BLUE,
	Color.LIGHT_BLUE, Color.LIGHT_GREEN, Color.LIGHT_YELLOW, Color.PINK, Color.INDIGO,
	Color.MAROON, Color.TEAL, Color.OLIVE, Color.LIME, Color.TURQUOISE, Color.SALMON,
	Color.SILVER, Color.PLUM, Color.DARK_GRAY, Color.AQUA, Color.FUCHSIA, Color.BLACK
]

func _ready() -> void:
	_create_color_buttons()

func _create_color_buttons() -> void:
	for color in _colors:
		var btn := Button.new()
		btn.custom_minimum_size = Vector2(40, 40)
		btn.flat = true

		# Bardzo ważne: ustawienie stylu przycisku (modulate nie działa dla tła!)
		var style := StyleBoxFlat.new()
		style.bg_color = color
		btn.add_theme_stylebox_override("normal", style)

		btn.tooltip_text = color.to_html()
		btn.pressed.connect(_on_color_button_pressed.bind(color))
		_button_panel.add_child(btn)

func _on_color_button_pressed(color: Color) -> void:
	_line_color = color
	print("Wybrano kolor:", color)

func _input(event: InputEvent) -> void:
	if event is InputEventMouseButton:
		if event.button_index == MOUSE_BUTTON_LEFT:
			_pressed = event.pressed
			if _pressed:
				_current_line = Line2D.new()
				_current_line.default_color = _line_color
				_current_line.width = 4
				_lines.add_child(_current_line)
				_current_line.add_point(event.position)
	elif event is InputEventMouseMotion and _pressed and _current_line:
		_current_line.add_point(event.position)
