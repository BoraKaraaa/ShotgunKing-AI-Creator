extends Node2D

onready var panel = $Panel
onready var label = $Panel/Label

# Declare member variables here. Examples:
# var a = 2
# var b = "text"

export var x:Vector2


# Called when the node enters the scene tree for the first time.
func _ready():
	print("feyyazin amcugunu yallucam bu aksam")
	label.text = "feyyazin amcugunu yallucam bu aksam";



# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
