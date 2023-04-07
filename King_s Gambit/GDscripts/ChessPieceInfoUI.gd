extends Control

var chessPieceUI = []
var currUI : Control = null

func _ready():
	
	for n in get_child_count():
		(get_child(n) as Control).visible = false
		chessPieceUI.append(get_child(n))
	
	(chessPieceUI[0] as Control).visible = true
	currUI = chessPieceUI[0]

func showUI(index:int) -> void:
	currUI.visible = false
	
	chessPieceUI[index].visible = true
	currUI = chessPieceUI[index]

