extends ItemList

onready var chessPieceInfoUI: Control = $"/root/MainScene/ChessPieceInfoUIHolder"

# Called when the node enters the scene tree for the first time.
func _ready():
	select(0)
	pass

func _on_ChessPieceList_item_selected(index):
	chessPieceInfoUI.showUI(index)
