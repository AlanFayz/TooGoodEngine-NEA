import TooGoodEngine 
import Entity

#brief:
#left, right, up, down indicate the bounds the camera can see
#position is the eye of the camera, up is the orientation of the camera 
#and front is the vector that points in front of the eye

class OrthographicCameraController:
	def __init__(self, cameraEntity: Entity.Entity):
		self.__handle = TooGoodEngine.internal_create_orthographic_camera_controller(cameraEntity.internal_get())

	def set_bottom(self, new_bottom: float) -> None:
		if self.__handle is not None:
			TooGoodEngine.internal_set_bottom_orthographic_camera_controller(self.__handle, new_bottom)

	def set_top(self, new_top: float) -> None: 
		if self.__handle is not None: 
			TooGoodEngine.internal_set_top_orthographic_camera_controller(self.__handle, new_top)
	
	def set_left(self, new_left: float) -> None: 
		if self.__handle is not None: 
			TooGoodEngine.internal_set_left_orthographic_camera_controller(self.__handle, new_left)

	def set_right(self, new_right: float) -> None:
		if self.__handle is not None: 
			TooGoodEngine.internal_set_right_orthographic_camera_controller(self.__handle, new_right)

	def update_position(self, new_position: tuple) -> None: 
		if self.__handle is not None: 
			TooGoodEngine.internal_update_position_orthographic_camera_controller(self.__handle, new_position[0], new_position[1], new_position[2])

	def update_up(self, new_up: tuple) -> None:
		if self.__handle is not None:
			TooGoodEngine.internal_update_up_orthographic_camera_controller(self.__handle, new_up[0], new_up[1], new_up[2])

	def update_front(self, new_front: tuple) -> None:
		if self.__handle is not None:
			TooGoodEngine.internal_update_front_orthographic_camera_controller(self.__handle, new_front[0], new_front[1], new_front[2])

