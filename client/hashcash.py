import hashlib
import base64
from bitstring import BitArray


class HashCashException(Exception):
    pass


class HashCash:
    def __init__(self, problem: str):
        self.problem = problem
        self.solution = ''

    def get_solution(self) -> str:
        return self.solution

    def solve(self) -> str:
        counter = 0
        first_delim = self.problem.find(':')
        if first_delim == -1:
            raise HashCashException('Invalid HashChallenge')

        second_delim = self.problem[first_delim + 1:].find(':')
        if second_delim == -1:
            raise HashCashException('Invalid HashChallenge')
        bits_to_zero = int(self.problem[first_delim + 1: first_delim + second_delim + 1])

        while True:
            hash_str = self.problem + ':' + base64.b64encode(str(counter).encode()).decode()
            hash_bytes = hashlib.sha256(hash_str.encode()).hexdigest()
            hash_bits = BitArray(hex=hash_bytes)
            if all([not hash_bit for hash_bit in hash_bits[:bits_to_zero]]):
                self.solution = hash_str
                break
            counter += 1
        return self.solution
