import TooGoodEngine 
import Entity

class PerspectiveCameraController:
	def __init__(self, cameraEntity: Entity.Entity):
		self.__handle = TooGoodEngine.internal_create_perspective_camera_controller(cameraEntity.internal_get())

	def set_fov(fov: float) -> None:
		if self.__handle is not None:
			TooGoodEngine.internal_set_fov_perspective_camera_controller(self.__handle, fov)

	def set_aspect_ratio(aspect_ratio: float) -> None:
		if self.__handle is not None: 
			TooGoodEngine.internal_set_aspect_ratio_perspective_camera_controller(self.__handle, aspect_ratio)

	def set_near(near: float) -> None:
		if self.__handle is not None: 
			TooGoodEngine.internal_set_near_perspective_camera_controller(self.__handle, near)

	def set_far(far: float) -> None:
		if self.__handle is not None: 
			TooGoodEngine.internal_set_far_perspective_camera_controller(self.__handle, far)

	def update_position(self, new_position: tuple) -> None: 
		if self.__handle is not None: 
			TooGoodEngine.internal_update_position_perspective_camera_controller(self.__handle, new_position[0], new_position[1], new_position[2])

	def update_up(self, new_up: tuple) -> None:
		if self.__handle is not None:
			TooGoodEngine.internal_update_up_perspective_camera_controller(self.__handle, new_up[0], new_up[1], new_up[2])

	def update_front(self, new_front: tuple) -> None:
		if self.__handle is not None:
			TooGoodEngine.internal_update_front_perspective_camera_controller(self.__handle, new_front[0], new_front[1], new_front[2])

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

