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