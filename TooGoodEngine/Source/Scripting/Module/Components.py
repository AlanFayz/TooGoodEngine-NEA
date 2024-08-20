import TooGoodEngine

class Transform:
	def __init__(self, handle):
		self.__handle = handle

	def translate(self, x: float, y: float, z: float) -> None:
		if self.__handle is not None:
			TooGoodEngine.internal_translate_transform(self.__handle, x, y, z)

	def rotate(self, x: float, y: float, z: float) -> None:
		if self.__handle is not None:
			TooGoodEngine.internal_rotate_transform(self.__handle, x, y, z)

	def scale(self, x: float, y: float, z: float) -> None:
		if self.__handle is not None:
			TooGoodEngine.internal_scale_transform(self.__handle, x, y, z)

	def get_translation(self) -> tuple:
		if self.__handle is not None:
			return TooGoodEngine.internal_get_translation_from_transform(self.__handle)

		return (-10000.0, -100000.0, -10000.0)

	def get_rotation(self) -> tuple:
		if self.__handle is not None:
			return TooGoodEngine.internal_get_rotation_from_transform(self.__handle)

		return (-10000.0, -100000.0, -10000.0)

	def get_scale(self) -> tuple:
		if self.__handle is not None:
			return TooGoodEngine.internal_get_scale_from_transform(self.__handle)

		return (-10000.0, -100000.0, -10000.0)

class MaterialAttributeCode:
	None_ = 0
	Ambient = 1
	Albedo = 2 
	AlbedoFactor = 3
	Metallic = 4
	MetallicFactor = 5
	Emission = 6
	EmissionFactor = 7
	Roughness = 8

class Material:
	def __init__(self, handle):
		self.__handle = handle

	def set_attribute(self, name: int, attribute: tuple) -> None:
		if self.__handle is not None and len(attribute) > 0 and len(attribute) < 5:
			if len(attribute) == 4:
				TooGoodEngine.internal_update_material_attribute(self.__handle, name, attribute[0], attribute[1], attribute[2], attribute[3])
			else:
				TooGoodEngine.internal_update_material_attribute(self.__handle, name, attribute[0], 0.0, 0.0, 0.0)

	def get_attribute(self, name: int) -> tuple:
		if self.__handle is not None:
			return TooGoodEngine.internal_get_material_attribute(self.__handle, name)