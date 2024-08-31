#include "Bindings.h"

#include "Project/Project.h"
#include "Utils/Input.h"

namespace TooGoodEngine {

	//
	// -------- Logging -------
	//

	PyObject* PythonBindings::LogMsg(PyObject* self, PyObject* args)
	{
		const char* val;
		if (!PyArg_ParseTuple(args, "s", &val))
			return nullptr;

		TGE_LOG_INFO(val);
		return Py_None;
	}

	PyObject* PythonBindings::LogWarn(PyObject* self, PyObject* args)
	{
		const char* val;
		if (!PyArg_ParseTuple(args, "s", &val))
			return nullptr;


		TGE_LOG_WARNING(val);
		return Py_None;
	}

	PyObject* PythonBindings::LogError(PyObject* self, PyObject* args)
	{
		const char* val;
		if (!PyArg_ParseTuple(args, "s", &val))
			return nullptr;

		TGE_LOG_ERROR(val);
		return Py_None;
	}

	//
	// -------- Input -------
	//

	PyObject* PythonBindings::IsKeyPressed(PyObject* self, PyObject* args)
	{
		int keyCode = 0;

		if (!PyArg_ParseTuple(args, "i", &keyCode))
			return nullptr;

		return PyBool_FromLong(Input::IsKeyPressed((KeyCode)keyCode));
	}

	PyObject* PythonBindings::IsKeyReleased(PyObject* self, PyObject* args)
	{
		int keyCode = 0;

		if (!PyArg_ParseTuple(args, "i", &keyCode))
			return nullptr;

		return PyBool_FromLong(Input::IsKeyReleased((KeyCode)keyCode));
	}

	PyObject* PythonBindings::IsKeyDown(PyObject* self, PyObject* args)
	{
		int keyCode = 0;

		if (!PyArg_ParseTuple(args, "i", &keyCode))
			return nullptr;

		return PyBool_FromLong(Input::IsKeyDown((KeyCode)keyCode));
	}

	PyObject* PythonBindings::IsMouseButtonPressed(PyObject* self, PyObject* args)
	{
		int mouseCode = 0;

		if (!PyArg_ParseTuple(args, "i", &mouseCode))
			return nullptr;

		return PyBool_FromLong(Input::IsMouseButtonPressed((ButtonCode)mouseCode));
	}

	PyObject* PythonBindings::IsMouseButtonReleased(PyObject* self, PyObject* args)
	{
		int mouseCode = 0;

		if (!PyArg_ParseTuple(args, "i", &mouseCode))
			return nullptr;

		return PyBool_FromLong(Input::IsMouseButtonReleased((ButtonCode)mouseCode));
	}

	PyObject* PythonBindings::IsMouseButtonDown(PyObject* self, PyObject* args)
	{
		int mouseCode = 0;

		if (!PyArg_ParseTuple(args, "i", &mouseCode))
			return nullptr;

		return PyBool_FromLong(Input::IsMouseButtonDown((ButtonCode)mouseCode));
	}

	PyObject* PythonBindings::GetMouseCoordinates(PyObject* self, PyObject* args)
	{
		double x = 0, y = 0;
		Input::GetMouseCoordinates(x, y);

		PyObject* pyX = PyFloat_FromDouble(x);
		PyObject* pyY = PyFloat_FromDouble(y);

		PyObject* tuple = PyTuple_New(2);

		PyTuple_SetItem(tuple, 0, pyX); 
		PyTuple_SetItem(tuple, 1, pyY);

		return tuple;
	}

	PyObject* PythonBindings::GetScrollWheel(PyObject* self, PyObject* args)
	{
		double x = 0, y = 0;
		Input::GetScrollWheel(x, y);

		PyObject* pyX = PyFloat_FromDouble(x);
		PyObject* pyY = PyFloat_FromDouble(y);

		PyObject* tuple = PyTuple_New(2);

		PyTuple_SetItem(tuple, 0, pyX);
		PyTuple_SetItem(tuple, 1, pyY);

		return tuple;
	}

	PyObject* PythonBindings::DisableCursor(PyObject* self, PyObject* args)
	{
		Input::DisableCursor();
		return Py_None;
	}

	PyObject* PythonBindings::EnableCursor(PyObject* self, PyObject* args)
	{
		Input::EnableCursor();
		return Py_None;
	}
	
	//
	// ---- Internal Entity -----
	//

	void PythonBindings::InternalCleanEntity(PyObject* capsule)
	{
		//not the best to be using heap memory directly. TODO: switch to using MemoryAllocator<> class
		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalEntity"));
		if (container)
			delete container;

		container = nullptr;
	}

	PyObject* PythonBindings::InternalCreateEntity(PyObject* self, PyObject* args)
	{
		const char* val;
		if (!PyArg_ParseTuple(args, "s", &val))
			return nullptr;

		Entity entity = g_SelectedProject->GetCurrentScene()->Add(std::string(val));

		Entity* container = new Entity(entity);
		return PyCapsule_New(container, "InternalEntity", InternalCleanEntity);
	}

	PyObject* PythonBindings::InternalGetEntityByName(PyObject* self, PyObject* args)
	{
		const char* val;
		if (!PyArg_ParseTuple(args, "s", &val))
			return nullptr;

		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();
		
		Entity entity = registry.GetEntityByName(val);
		if (!entity)
			return nullptr;

		Entity* container = new Entity(entity);
		return PyCapsule_New(container, "InternalEntity", InternalCleanEntity);
	}

	PyObject* PythonBindings::InternalGetEntityName(PyObject* self, PyObject* args)
	{
		PyObject* capsule;
		if (!PyArg_ParseTuple(args, "O", &capsule))
			return nullptr;

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalEntity"));
		if (!container)
			return nullptr;

		return PyUnicode_FromString(container->GetName().c_str());
	}

	PyObject* PythonBindings::InternalGetEntityID(PyObject* self, PyObject* args)
	{
		PyObject* capsule;
		if (!PyArg_ParseTuple(args, "O", &capsule))
			return nullptr;

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalEntity"));
		if (!container)
			return nullptr;

		return PyLong_FromUnsignedLongLong((uint64_t)container->GetID());
	}

	PyObject* PythonBindings::InternalAddComponentToEntity(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		const char* name = nullptr;
		if (!PyArg_ParseTuple(args, "Os", &capsule, &name))
			return nullptr;

		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();
		auto renderer  = g_SelectedProject->GetCurrentScene()->GetSceneRenderer();
		
		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalEntity"));

		if (!container)
			return nullptr;
		

		std::string stringName = std::string(name);

		if (stringName == "Transform" && !registry.HasComponent<TransformComponent>(*container))
			registry.EmplaceComponent<TransformComponent>(*container);
		
		else if (stringName == "Material" && !registry.HasComponent<MaterialComponent>(*container))
		{
			MaterialComponent component{};
			component.ID = renderer->CreateMaterial();
			component.Renderer = renderer;
			registry.AddComponent(*container, component);

		}
		
		else if (stringName == "Directional Light" && !registry.HasComponent<DirectionalLightComponent>(*container))
			registry.EmplaceComponent<DirectionalLightComponent>(*container);
		
		else if (stringName == "Point Light" && !registry.HasComponent<PointLightComponent>(*container))
			registry.EmplaceComponent<PointLightComponent>(*container);
		
		else if (stringName == "Quad")
		{
			MeshComponent component;
			component.ID = 0;
			component.PathToSource = "##Quad";

			registry.AddComponent(*container, component);
		}

		else if (stringName == "Cube")
		{
			MeshComponent component;
			component.ID = 1;
			component.PathToSource = "##Cube";

			registry.AddComponent(*container, component);
		}

		return Py_None;
	}

	PyObject* PythonBindings::InternalRemoveComponentFromEntity(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		const char* name = nullptr;
		if (!PyArg_ParseTuple(args, "Os", &capsule, &name))
			return nullptr;

		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalEntity"));

		if (!container)
			return nullptr;

		if (name == "Transform" && registry.HasComponent<TransformComponent>(*container))
			registry.RemoveComponent<TransformComponent>(*container);

		else if (name == "Material" && registry.HasComponent<MaterialComponent>(*container))
			registry.RemoveComponent<MaterialComponent>(*container);

		else if (name == "Directional Light" && registry.HasComponent<DirectionalLightComponent>(*container))
			registry.RemoveComponent<DirectionalLightComponent>(*container);

		else if (name == "Point Light" && registry.HasComponent<PointLightComponent>(*container))
			registry.RemoveComponent<PointLightComponent>(*container);

		else if (name == "Quad" || name == "Cube" && registry.HasComponent<MeshComponent>(*container))
			registry.RemoveComponent<MeshComponent>(*container);

		return Py_None;
	}

	PyObject* PythonBindings::InternalRemoveEntityFromTree(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		if (!PyArg_ParseTuple(args, "O", &capsule))
			return nullptr;

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalEntity"));

		if (!container)
			return nullptr;

		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();
		registry.RemoveEntity(*container);

		return Py_None;
	}

	PyObject* PythonBindings::InternalGetComponentFromEntity(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		const char* name = nullptr;
		if (!PyArg_ParseTuple(args, "Os", &capsule, &name))
			return nullptr;

		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalEntity"));

		if (!container || !*container)
			return nullptr;

		std::string stringName = name;

		if (stringName == "Transform" && registry.HasComponent<TransformComponent>(*container))
		{
			Entity* componentContainer = container;

			return PyCapsule_New(componentContainer, "InternalTransform", InternalCleanTransform);
		}

		else if (stringName == "Material" && registry.HasComponent<MaterialComponent>(*container))
		{
			Entity* componentContainer = container;

			return PyCapsule_New(componentContainer, "InternalMaterial", InternalCleanMaterial);
		}

		else if (stringName == "Directional Light" && registry.HasComponent<DirectionalLightComponent>(*container))
		{
			Entity* componentContainer = container;

			return PyCapsule_New(componentContainer, "InternalDirectionalLight", InternalCleanDirectionalLight);
		}

		else if (stringName == "Point Light" && registry.HasComponent<PointLightComponent>(*container))
		{
			Entity* componentContainer = container;
			return PyCapsule_New(componentContainer, "InternalPointLight", InternalCleanPointLight);
		}
	
		PyErr_Format(PyExc_NameError, "Not a valid component %s", name);
		return nullptr;
	}

	//
	// ---- Internal Transform -----
	//
	
	void PythonBindings::InternalCleanTransform(PyObject* capsule)
	{
		
	}

	PyObject* PythonBindings::InternalTranslateTransform(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		if (!PyArg_ParseTuple(args, "Offf", &capsule, &x, &y, &z))
			return nullptr;

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalTransform"));
		
		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();

		if (!container || !*container || !registry.HasComponent<TransformComponent>(*container))
			return nullptr;

		auto& component = registry.GetComponent<TransformComponent>(*container);
		component.Translate({ x, y, z });

		return Py_None;
	}

	PyObject* PythonBindings::InternalRotateTransform(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		if (!PyArg_ParseTuple(args, "Offf", &capsule, &x, &y, &z))
			return nullptr;

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalTransform"));

		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();

		if (!container || !*container || !registry.HasComponent<TransformComponent>(*container))
			return nullptr;

		auto& component = registry.GetComponent<TransformComponent>(*container);
		component.Rotate({ x, y, z });

		return Py_None;
	}

	PyObject* PythonBindings::InternalScaleTransform(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		if (!PyArg_ParseTuple(args, "Offf", &capsule, &x, &y, &z))
			return nullptr;

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalTransform"));

		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();

		if (!container || !*container || !registry.HasComponent<TransformComponent>(*container))
			return nullptr;

		auto& component = registry.GetComponent<TransformComponent>(*container);
		component.Scale({ x, y, z });

		return Py_None;
	}

	PyObject* PythonBindings::InternalGetTranslationFromTransform(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;

		if (!PyArg_ParseTuple(args, "O", &capsule))
			return nullptr;

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalTransform"));

		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();

		if (!container || !*container || !registry.HasComponent<TransformComponent>(*container))
			return nullptr;

		auto& component = registry.GetComponent<TransformComponent>(*container);
		auto& position = component.GetPosition();

		PyObject* pyX = PyFloat_FromDouble(position.x);
		PyObject* pyY = PyFloat_FromDouble(position.y);
		PyObject* pyZ = PyFloat_FromDouble(position.z);

		PyObject* tuple = PyTuple_New(3);

		PyTuple_SetItem(tuple, 0, pyX);
		PyTuple_SetItem(tuple, 1, pyY);
		PyTuple_SetItem(tuple, 2, pyZ);

		return tuple;
	}

	PyObject* PythonBindings::InternalGetRotationFromTransform(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;

		if (!PyArg_ParseTuple(args, "O", &capsule))
			return nullptr;

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalTransform"));

		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();

		if (!container || !*container || !registry.HasComponent<TransformComponent>(*container))
			return nullptr;

		auto& component = registry.GetComponent<TransformComponent>(*container);
		auto& rotation = component.GetRotation();

		PyObject* pyX = PyFloat_FromDouble(rotation.x);
		PyObject* pyY = PyFloat_FromDouble(rotation.y);
		PyObject* pyZ = PyFloat_FromDouble(rotation.z);

		PyObject* tuple = PyTuple_New(3);

		PyTuple_SetItem(tuple, 0, pyX);
		PyTuple_SetItem(tuple, 1, pyY);
		PyTuple_SetItem(tuple, 2, pyZ);

		return tuple;
	}

	PyObject* PythonBindings::InternalGetScaleFromTransform(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;

		if (!PyArg_ParseTuple(args, "O", &capsule))
			return nullptr;

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalTransform"));

		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();

		if (!container || !*container || !registry.HasComponent<TransformComponent>(*container))
			return nullptr;

		auto& component = registry.GetComponent<TransformComponent>(*container);
		auto& scale = component.GetScale();

		PyObject* pyX = PyFloat_FromDouble(scale.x);
		PyObject* pyY = PyFloat_FromDouble(scale.y);
		PyObject* pyZ = PyFloat_FromDouble(scale.z);

		PyObject* tuple = PyTuple_New(3);

		PyTuple_SetItem(tuple, 0, pyX);
		PyTuple_SetItem(tuple, 1, pyY);
		PyTuple_SetItem(tuple, 2, pyZ);

		return tuple;
	}

	void PythonBindings::InternalCleanMaterial(PyObject* capsule)
	{
	}

	enum class InternalMaterialAttribute
	{
		None = 0, Ambient, Albedo, Metallic,
		Emission, EmissionFactor, Roughness
	};

	PyObject* PythonBindings::InternalUpdateMaterialAttribute(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		int materialCode = 0;

		float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;

		if (!PyArg_ParseTuple(args, "Oiffff", &capsule, &materialCode, &r, &g, &b, &a))
			return nullptr;

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalMaterial"));

		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();
		auto  renderer = g_SelectedProject->GetCurrentScene()->GetSceneRenderer();

		if (!container || !*container || !registry.HasComponent<TransformComponent>(*container))
			return nullptr;

		MaterialComponent& component = registry.GetComponent<MaterialComponent>(*container);
		auto& materialInfo = component.Renderer->GetMaterialInfo(component.ID);

		InternalMaterialAttribute attribute = (InternalMaterialAttribute)materialCode;

		switch (attribute)
		{
			case  InternalMaterialAttribute::Ambient:		 materialInfo.Ambient   = { r, g, b, a }; break;
			case  InternalMaterialAttribute::Albedo:		 materialInfo.Albedo    = { r, g, b, a }; break;
			case  InternalMaterialAttribute::Emission:		 materialInfo.Emission  = { r, g, b, a }; break;
			case  InternalMaterialAttribute::Metallic:	     materialInfo.Metallic  = r; break;
			case  InternalMaterialAttribute::Roughness:	     materialInfo.Roughness = r; break;
			case  InternalMaterialAttribute::EmissionFactor: materialInfo.EmissionFactor = r;  break;
			case  InternalMaterialAttribute::None:
			default:
				TGE_LOG_WARNING("no attribute edited of material");
				break;
		}

		renderer->ModifyMaterial(component.ID, materialInfo);

		return Py_None;
	}

	PyObject* PythonBindings::InternalGetMaterialAttribute(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		int materialCode = 0;


		if (!PyArg_ParseTuple(args, "Oi", &capsule, &materialCode))
			return nullptr;

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalMaterial"));

		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();

		if (!container || !*container || !registry.HasComponent<TransformComponent>(*container))
			return nullptr;

		MaterialComponent& component = registry.GetComponent<MaterialComponent>(*container);
		auto& materialInfo = component.Renderer->GetMaterialInfo(component.ID);


		InternalMaterialAttribute attribute = (InternalMaterialAttribute)materialCode;

		PyObject* pyR = nullptr;
		PyObject* pyG = nullptr;
		PyObject* pyB = nullptr;
		PyObject* pyA = nullptr;

		switch (attribute)
		{
		case  InternalMaterialAttribute::Ambient:	

			 pyR = PyFloat_FromDouble(materialInfo.Ambient.r);
			 pyG = PyFloat_FromDouble(materialInfo.Ambient.g);
			 pyB = PyFloat_FromDouble(materialInfo.Ambient.b);
			 pyA = PyFloat_FromDouble(materialInfo.Ambient.a);


			break;
		case  InternalMaterialAttribute::Albedo:		 

			pyR = PyFloat_FromDouble(materialInfo.Albedo.r);
			pyG = PyFloat_FromDouble(materialInfo.Albedo.g);
			pyB = PyFloat_FromDouble(materialInfo.Albedo.b);
			pyA = PyFloat_FromDouble(materialInfo.Albedo.a);


			break;
		case  InternalMaterialAttribute::Metallic:	     

			pyR = PyFloat_FromDouble(materialInfo.Metallic);

			break;

		case  InternalMaterialAttribute::Emission:		 
			
			pyR = PyFloat_FromDouble(materialInfo.Emission.r);
			pyG = PyFloat_FromDouble(materialInfo.Emission.g);
			pyB = PyFloat_FromDouble(materialInfo.Emission.b);
			pyA = PyFloat_FromDouble(materialInfo.Emission.a);


			break;
		case  InternalMaterialAttribute::Roughness:	     
			
			pyR = PyFloat_FromDouble(materialInfo.Roughness);

			break; 
		case  InternalMaterialAttribute::EmissionFactor: 

			pyR = PyFloat_FromDouble(materialInfo.EmissionFactor);

			break;
		case  InternalMaterialAttribute::None:
		default:
			TGE_LOG_WARNING("material attribute not found");
			break;
		}

		if (pyR && pyG && pyB && pyA)
		{
			PyObject* tuple = PyTuple_New(4);

			PyTuple_SetItem(tuple, 0, pyR);
			PyTuple_SetItem(tuple, 1, pyG);
			PyTuple_SetItem(tuple, 2, pyB);
			PyTuple_SetItem(tuple, 3, pyA);

			return tuple;
		}
		else if (pyR)
		{
			//still returning a tuple just so we can have a consistent type
			//for the game dev

			PyObject* tuple = PyTuple_New(1);
			PyTuple_SetItem(tuple, 0, pyR);

			return tuple;
		}


		return nullptr;
	}

	void PythonBindings::InternalCleanPointLight(PyObject* capsule)
	{
	}

	PyObject* PythonBindings::InternalTranslatePointLight(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float x = 0.0f, y = 0.0f, z = 0.0f;

		if (!PyArg_ParseTuple(args, "Offf", &capsule, &x, &y, &z))
			return nullptr;

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalPointLight"));

		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();

		if (!container || !*container || !registry.HasComponent<PointLightComponent>(*container))
			return nullptr;

		PointLightComponent& component = registry.GetComponent<PointLightComponent>(*container);
		component.Position += glm::vec3(x, y, z);

		return Py_None;
	}

	PyObject* PythonBindings::InternalUpdatePointLightColor(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float r = 0.0f, g = 0.0f, b = 0.0f;

		if (!PyArg_ParseTuple(args, "Offf", &capsule, &r, &g, &b))
			return nullptr;

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalPointLight"));

		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();

		if (!container || !*container || !registry.HasComponent<PointLightComponent>(*container))
			return nullptr;

		PointLightComponent& component = registry.GetComponent<PointLightComponent>(*container);
		component.Color = { r, g ,b, 1.0 };

		return Py_None;
	}

	PyObject* PythonBindings::InternalUpdatePointLightIntensity(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float intensity = 0.0f;

		if (!PyArg_ParseTuple(args, "Of", &capsule, &intensity))
			return nullptr;

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalPointLight"));

		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();

		if (!container || !*container || !registry.HasComponent<PointLightComponent>(*container))
			return nullptr;

		PointLightComponent& component = registry.GetComponent<PointLightComponent>(*container);
		component.Intensity = intensity;

		return Py_None;
	}

	PyObject* PythonBindings::InternalUpdatePointLightRadius(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float radius = 0.0f;

		if (!PyArg_ParseTuple(args, "Of", &capsule, &radius))
			return nullptr;

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalPointLight"));

		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();

		if (!container || !*container || !registry.HasComponent<PointLightComponent>(*container))
			return nullptr;

		PointLightComponent& component = registry.GetComponent<PointLightComponent>(*container);
		component.Radius = radius;

		return Py_None;
	}

	void PythonBindings::InternalCleanDirectionalLight(PyObject* capsule)
	{
	}

	PyObject* PythonBindings::InternalUpdateDirectionalLightDirection(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float x = 0.0f, y = 0.0f, z = 0.0f;

		if (!PyArg_ParseTuple(args, "Offf", &capsule, &x, &y, &z))
			return nullptr;

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalDirectionalLight"));

		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();

		if (!container || !*container || !registry.HasComponent<DirectionalLightComponent>(*container))
			return nullptr;

		DirectionalLightComponent& component = registry.GetComponent<DirectionalLightComponent>(*container);
		component.Direction = glm::vec3(x, y, z);

		return Py_None;
	}

	PyObject* PythonBindings::InternalUpdateDirectionalLightColor(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float r = 0.0f, g = 0.0f, b = 0.0f;

		if (!PyArg_ParseTuple(args, "Offf", &capsule, &r, &g, &b))
			return nullptr;

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalDirectionalLight"));

		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();

		if (!container || !*container || !registry.HasComponent<DirectionalLightComponent>(*container))
			return nullptr;

		DirectionalLightComponent& component = registry.GetComponent<DirectionalLightComponent>(*container);
		component.Color = glm::vec4(r, g, b, 1.0f);
	
		return Py_None;
	}

	PyObject* PythonBindings::InternalUpdateDirectionalLightIntensity(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float intensity = 0.0f;

		if (!PyArg_ParseTuple(args, "Of", &capsule, &intensity))
			return nullptr;

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalDirectionalLight"));

		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();

		if (!container || !*container || !registry.HasComponent<DirectionalLightComponent>(*container))
			return nullptr;

		DirectionalLightComponent& component = registry.GetComponent<DirectionalLightComponent>(*container);
		component.Intensity = intensity;

		return Py_None;
	}

	void PythonBindings::InternalCleanOrthographicCameraController(PyObject* capsule)
	{
		OrthographicCameraController* container = (OrthographicCameraController*)(PyCapsule_GetPointer(capsule, "InternalOrthographicCameraController"));
		
		if (container)
			delete container;

		container = nullptr;
	}

	PyObject* PythonBindings::InternalCreateOrthographicCameraController(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;

		if (!PyArg_ParseTuple(args, "O", &capsule))
			return nullptr;

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalEntity"));
		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();

		if (!container || !*container || !registry.HasComponent<OrthographicCameraComponent>(*container))
			return nullptr;

		auto& component = registry.GetComponent<OrthographicCameraComponent>(*container);

		OrthographicCameraController* cameraControllerContainer = new OrthographicCameraController(component.Camera);
		return PyCapsule_New(cameraControllerContainer, "InternalOrthographicCameraController", InternalCleanOrthographicCameraController);
	}

	PyObject* PythonBindings::InternalSetBottomOrthographicCameraController(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float bottom = 0.0f;

		if (!PyArg_ParseTuple(args, "Of", &capsule, &bottom))
			return nullptr;

		OrthographicCameraController* container = (OrthographicCameraController*)(PyCapsule_GetPointer(capsule, "InternalOrthographicCameraController"));
	
		if (!container)
			return nullptr;
		
		container->SetBottom(bottom);

		return Py_None;
	}

	PyObject* PythonBindings::InternalSetTopOrthographicCameraController(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float top = 0.0f;

		if (!PyArg_ParseTuple(args, "Of", &capsule, &top))
			return nullptr;

		OrthographicCameraController* container = (OrthographicCameraController*)(PyCapsule_GetPointer(capsule, "InternalOrthographicCameraController"));
		
		if (!container)
			return nullptr;
		
		container->SetTop(top);

		return Py_None;
	}

	PyObject* PythonBindings::InternalSetLeftOrthographicCameraController(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float left = 0.0f;

		if (!PyArg_ParseTuple(args, "Of", &capsule, &left))
			return nullptr;

		OrthographicCameraController* container = (OrthographicCameraController*)(PyCapsule_GetPointer(capsule, "InternalOrthographicCameraController"));

		if (!container)
			return nullptr;

		container->SetLeft(left);

		return Py_None;
	}

	PyObject* PythonBindings::InternalSetRightOrthographicCameraController(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float right = 0.0f;

		if (!PyArg_ParseTuple(args, "Of", &capsule, &right))
			return nullptr;

		OrthographicCameraController* container = (OrthographicCameraController*)(PyCapsule_GetPointer(capsule, "InternalOrthographicCameraController"));

		if (!container)
			return nullptr;

		container->SetTop(right);

		return Py_None;
	}

	PyObject* PythonBindings::InternalUpdatePositionOrthographicCameraController(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float x = 0.0f, y = 0.0f, z = 0.0f;

		if (!PyArg_ParseTuple(args, "Offf", &capsule, &x, &y, &z))
			return nullptr;

		OrthographicCameraController* container = (OrthographicCameraController*)(PyCapsule_GetPointer(capsule, "InternalOrthographicCameraController"));

		if (!container)
			return nullptr;

		container->UpdatePosition(glm::vec3(x, y, z));

		return Py_None;
	}

	PyObject* PythonBindings::InternalUpdateUpOrthographicCameraController(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float x = 0.0f, y = 0.0f, z = 0.0f;

		if (!PyArg_ParseTuple(args, "Offf", &capsule, &x, &y, &z))
			return nullptr;

		OrthographicCameraController* container = (OrthographicCameraController*)(PyCapsule_GetPointer(capsule, "InternalOrthographicCameraController"));

		if (!container)
			return nullptr;

		container->UpdateUp(glm::vec3(x, y, z));

		return Py_None;
	}

	PyObject* PythonBindings::InternalUpdateFrontOrthographicCameraController(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float x = 0.0f, y = 0.0f, z = 0.0f;

		if (!PyArg_ParseTuple(args, "Offf", &capsule, &x, &y, &z))
			return nullptr;

		OrthographicCameraController* container = (OrthographicCameraController*)(PyCapsule_GetPointer(capsule, "InternalOrthographicCameraController"));

		if (!container)
			return nullptr;

		container->UpdateFront(glm::vec3(x, y, z));
		return Py_None;
	}
	
	
	void PythonBindings::InternalCleanPerspectiveCameraController(PyObject* capsule)
	{
		PerspectiveCameraController* container = (PerspectiveCameraController*)(PyCapsule_GetPointer(capsule, "InternalPerspectiveCameraController"));

		if (container)
			delete container;
	}

	PyObject* PythonBindings::InternalCreatePerpsectiveCameraController(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;

		if (!PyArg_ParseTuple(args, "O", &capsule))
			return nullptr;

		Entity* container = (Entity*)(PyCapsule_GetPointer(capsule, "InternalEntity"));
		auto& registry = g_SelectedProject->GetCurrentScene()->GetRegistry();

		if (!container || !*container || !registry.HasComponent<PerspectiveCameraComponent>(*container))
			return nullptr;

		auto& component = registry.GetComponent<PerspectiveCameraComponent>(*container);

		PerspectiveCameraController* cameraControllerContainer = new PerspectiveCameraController(component.Camera);
		return PyCapsule_New(cameraControllerContainer, "InternalPerspectiveCameraController", InternalCleanPerspectiveCameraController);
	}
	
	PyObject* PythonBindings::InternalSetFovPerspectiveCameraController(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float fov = 0.0f;

		if (!PyArg_ParseTuple(args, "Of", &capsule, &fov))
			return nullptr;

		PerspectiveCameraController* container = (PerspectiveCameraController*)(PyCapsule_GetPointer(capsule, "InternalPerspectiveCameraController"));

		if (!container)
			return nullptr;

		container->SetFov(fov);
		return Py_None;
	}
	PyObject* PythonBindings::InternalSetAspectRatioPerpspectiveCameraController(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float aspectRatio = 0.0f;

		if (!PyArg_ParseTuple(args, "Of", &capsule, &aspectRatio))
			return nullptr;

		PerspectiveCameraController* container = (PerspectiveCameraController*)(PyCapsule_GetPointer(capsule, "InternalPerspectiveCameraController"));

		if (!container)
			return nullptr;

		container->SetAspectRatio(aspectRatio);
		return Py_None;
	}
	PyObject* PythonBindings::InternalSetNearPerspectiveCameraController(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float near = 0.0f;

		if (!PyArg_ParseTuple(args, "Of", &capsule, &near))
			return nullptr;

		PerspectiveCameraController* container = (PerspectiveCameraController*)(PyCapsule_GetPointer(capsule, "InternalPerspectiveCameraController"));

		if (!container)
			return nullptr;

		container->SetNear(near);
		return Py_None;
	}
	PyObject* PythonBindings::InternalSetFarPerspectiveCameraController(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float far = 0.0f;

		if (!PyArg_ParseTuple(args, "Of", &capsule, &far))
			return nullptr;

		PerspectiveCameraController* container = (PerspectiveCameraController*)(PyCapsule_GetPointer(capsule, "InternalPerspectiveCameraController"));

		if (!container)
			return nullptr;

		container->SetFar(far);
		return Py_None;
	}
	PyObject* PythonBindings::InternalUpdatePositionPerspectiveCameraController(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float x = 0.0f, y = 0.0f, z = 0.0f;

		if (!PyArg_ParseTuple(args, "Offf", &capsule, &x, &y, &z))
			return nullptr;

		PerspectiveCameraController* container = (PerspectiveCameraController*)(PyCapsule_GetPointer(capsule, "InternalPerspectiveCameraController"));

		if (!container)
			return nullptr;

		container->UpdatePosition(glm::vec3(x, y ,z));
		return Py_None;
	}
	PyObject* PythonBindings::InternalUpdateUpPerspectiveCameraController(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float x = 0.0f, y = 0.0f, z = 0.0f;

		if (!PyArg_ParseTuple(args, "Offf", &capsule, &x, &y, &z))
			return nullptr;

		PerspectiveCameraController* container = (PerspectiveCameraController*)(PyCapsule_GetPointer(capsule, "InternalPerspectiveCameraController"));

		if (!container)
			return nullptr;

		container->UpdateUp(glm::vec3(x, y, z));
		return Py_None;
	}
	PyObject* PythonBindings::InternalUpdateFrontPerspectiveCameraController(PyObject* self, PyObject* args)
	{
		PyObject* capsule = nullptr;
		float x = 0.0f, y = 0.0f, z = 0.0f;

		if (!PyArg_ParseTuple(args, "Offf", &capsule, &x, &y, &z))
			return nullptr;
		
		PerspectiveCameraController* container = (PerspectiveCameraController*)(PyCapsule_GetPointer(capsule, "InternalPerspectiveCameraController"));

		if (!container)
			return nullptr;

		container->UpdateFront(glm::vec3(x, y, z));
		return Py_None;
	}
}