from typing import List, Iterable


def frange(start: float, stop: float, step: float) -> Iterable[float]:
    if start > stop:
        yield from (start - x for x in frange(0, start - stop, -step))
    else:
        while start < stop:
            yield start
            start += step


class Function:
    def __init__(self) -> None:
        super().__init__()

    def map(self, x: float) -> float:
        return 0

    def derive(self):
        return Function()


class Polynomial(Function):
    def __init__(self, coefficients: List[float]) -> None:
        super().__init__()
        self.coefficients = coefficients

    def map(self, x: float) -> float:
        value = 0
        for c in self.coefficients:
            value = value * x + c
        return value

    def derive(self):
        degree = len(self.coefficients) - 1
        return Polynomial([self.coefficients[i] * (degree - i) for i in range(degree)])
