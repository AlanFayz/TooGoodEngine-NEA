import math

#todo: this function should be implemented in c++ for better performance
#currently is faster than numpy somehow

class Vector:
    def __init__(self, data):
        if isinstance(data, Vector):
            self.__data = data.__data[:]
        else:
            self.__data = list(data)

    def magnitude(self) -> float:
        return math.sqrt(sum(x**2 for x in self.__data))

    def size(self) -> int:
        return len(self.__data)

    def __getitem__(self, index: int) -> float:
        return self.__data[index]

    def __setitem__(self, index: int, value: float):
        self.__data[index] = value

    def __str__(self) -> str:
        return str(self.__data)

    def __add__(self, other) -> 'Vector':
        if not isinstance(other, Vector):
            raise TypeError("other type should be vector")

        minSize = min(other.size(), self.size())
        result = self.__data[:]

        for i in range(minSize):
            result[i] += other[i]

        return Vector(result)

    def __sub__(self, other) -> 'Vector':
        if not isinstance(other, Vector):
            raise TypeError("other type should be vector")

        minSize = min(other.size(), self.size())
        result = self.__data[:]

        for i in range(minSize):
            result[i] -= other[i]

        return Vector(result)

    def __mul__(self, other) -> 'Vector':
        if isinstance(other, Vector):
            minSize = min(other.size(), self.size())
            result = self.__data[:]

            for i in range(minSize):
                result[i] *= other[i]

            return Vector(result)
        elif isinstance(other, float):
            result = [x * other for x in self.__data]
            return Vector(result)
        else:
            raise TypeError("other type should be vector or float")

    def __truediv__(self, other) -> 'Vector':
        if isinstance(other, Vector):
            minSize = min(other.size(), self.size())
            result = self.__data[:]

            for i in range(minSize):
                if other[i] == 0:
                    raise ZeroDivisionError("division by zero")

                result[i] /= other[i]

            return Vector(result)
        elif isinstance(other, float):
            if other == 0:
                raise ZeroDivisionError("division by zero")

            result = [x / other for x in self.__data]
            return Vector(result)
        else:
            raise TypeError("other type should be vector or float")

    def __floordiv__(self, other) -> 'Vector':
        if not isinstance(other, Vector):
            raise TypeError("other type should be vector")

        minSize = min(other.size(), self.size())
        result = self.__data[:]

        for i in range(minSize):
            if other[i] == 0:
                raise ZeroDivisionError("division by zero")

            result[i] //= other[i]

        return Vector(result)


def cross2(vector1: Vector, vector2: Vector) -> float:
	if  vector1.size() != 2 or vector2.size() != 2:
		raise ValueError("both vectors must have 2 components")

	return vector1[0] * vector2[1] - vector1[1] * vector2[0]


def cross(vector1: Vector, vector2: Vector) -> Vector:
	if vector1.size() != 3 or vector2.size() != 3:
		raise ValueError("both vectors must have 3 components")

	x = vector1[1] * vector2[2] - vector1[2] * vector2[1]
	y = vector1[2] * vector2[0] - vector1[0] * vector2[2]
	z = vector1[0] * vector2[1] - vector1[1] * vector2[0]

	return Vector((x, y, z))

def dot(vector1: Vector, vector2: Vector) -> float:
	if vector1.size() != vector2.size():
		raise ValueError("both vectors must have the same size") 

	return sum(x * y for x,y in zip(vector1, vector2)) 

def normalize(vector: Vector) -> Vector:
	magnitude = vector.magnitude()
	if magnitude == 0.0:
		raise ValueError("cannot normalize a zero vector")

	return Vector(vector) / magnitude

def distance(vector1: Vector, vector2: Vector) -> float:
	difference = vector2 - vector1
	return difference.magnitude()

