import TooGoodEngine

#NOTE: users should never try to construct
#a component by itself. always construct it
#from an entity and then retrieve it, so
#never use these constructors

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
	Metallic = 3
	Emission = 4
	EmissionFactor = 5
	Roughness = 6

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


class PointLight:
	def __init__(self, handle):
		self.__handle = handle

	def translate_position(self, offset: tuple) -> None:
		if self.__handle is not None:
			TooGoodEngine.internal_translate_point_light(self.__handle, offset[0], offset[1], offset[2])

	def update_color(self, new_color: tuple) -> None:
		if self.__handle is not None:
			TooGoodEngine.internal_update_point_light_color(self.__handle, new_color[0], new_color[1], new_color[2])

	def update_intensity(self, new_intensity: float) -> None:
		if self.__handle is not None:
			TooGoodEngine.internal_update_point_light_intensity(self.__handle, new_intensity)

	def update_radius(self, new_radius: float) -> None:
		if self.__handle is not None:
			TooGoodEngine.internal_update_point_light_radius(self.__handle, new_radius)


class DirectionalLight:
	def __init__(self, handle):
		self.__handle = handle 

	def update_direction(self, new_direction: tuple) -> None: 
		if self.__handle is not None:
			TooGoodEngine.internal_update_directional_light_direction(self.__handle, new_direction[0], new_direction[1], new_direction[2])

	def update_color(self, new_color: tuple) -> None:
		if self.__handle is not None:
			TooGoodEngine.internal_update_directional_light_color(self.__handle, new_color[0], new_color[1]. new_color[2])
		
	def update_intensity(self, new_intensity: float) -> None: 
		if self.__handle is not None:
			TooGoodEngine.internal_update_directional_light_intensity(self.__handle, new_intensity)


