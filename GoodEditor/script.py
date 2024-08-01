import TooGoodEngine as Engine

def on_create():
	Engine.log("hello world")

def on_update(delta):
	Engine.log(str(delta))