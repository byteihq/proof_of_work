import json
import requests


class ApiExceptions(Exception):
    pass


class ApiRequest:
    def __init__(self, url: str, city='', hash_cash=''):
        self.url = url
        self.hash_cash = hash_cash
        self.city = city

    def ping(self) -> requests.Response:
        return requests.get(self.url)

    def set_city(self, city: str) -> None:
        self.city = city

    def set_hash_cash(self, hash_cash: str) -> None:
        self.hash_cash = hash_cash

    def send(self) -> requests.Response:
        if len(self.city) == 0:
            raise ApiExceptions('City is undefined!')

        return requests.post(self.url + '/weather', json.dumps({'city': self.city, 'HashCash': self.hash_cash}))
