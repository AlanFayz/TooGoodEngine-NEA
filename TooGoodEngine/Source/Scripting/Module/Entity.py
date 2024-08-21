import TooGoodEngine
import Components

g_null_entity = 0xFFFFFFFFFFFFFFFF

g_component_dictionary = {
	"Transform":		 Components.Transform,
	"Material":			 Components.Material, 
	"Directional Light": Components.DirectionalLight,
	"Point Light":       Components.PointLight }

class Entity:
	def __init__(self, name: str):
		self.__handle = TooGoodEngine.internal_create_entity(name)

	def __init__(self, handle):
		self.__handle = handle

	def internal_get(self):
		return self.__handle

	def get_name(self) -> str:
		if self.__handle is not None:
			return TooGoodEngine.internal_get_entity_name(self.__handle)

		return "null entity"

	def get_id(self) -> int:
		if self.__handle is not None:
			return TooGoodEngine.internal_get_entity_id(self.__handle)

		return g_null_entity

	def add_component(self, name: str) -> None:
		if self.__handle is not None:
			TooGoodEngine.internal_add_component_to_entity(self.__handle, name)
	
	def remove_component(self, name: str) -> None:
		if self.__handle is not None:
			TooGoodEngine.internal_remove_component_from_entity(self.__handle, name)

	def get_component(self, name: str) -> any:
		if self.__handle is not None and name in g_component_dictionary:
			handle = TooGoodEngine.internal_get_component_from_entity(self.__handle, name)

			if handle is None:
				return None 

			return  g_component_dictionary[name](handle)

		return None

	def remove_from_tree(self) -> None:
		TooGoodEngine.internal_remove_entity_from_tree(self.__handle)
		self.__handle = None


def get_entity_by_name(name: str):
	handle = TooGoodEngine.internal_get_entity_by_name(name)
	if handle is not None:
		return Entity(handle)

	return None