import json
import requests


class ApiExceptions(Exception):
    pass


class ApiRequest:
    def __init__(self, url: str, wait_for_response=False, city='', hash_cash=''):
        self.url = url
        self.hash_cash = hash_cash
        self.city = city
        self.wait_for_response = wait_for_response

    def ping(self) -> requests.Response:
        return requests.get(self.url)

    def set_city(self, city: str) -> None:
        self.city = city

    def set_hash_cash(self, hash_cash: str) -> None:
        self.hash_cash = hash_cash

    def send(self) -> requests.Response:
        if len(self.city) == 0:
            raise ApiExceptions('City is undefined!')
        req = {'city': self.city}
        if len(self.hash_cash) != 0:
            req['HashCash'] = self.hash_cash
        
        if self.wait_for_response:
            return requests.post(self.url + '/weather', json.dumps(req), allow_redirects=True)
        try:
            return requests.post(self.url + '/weather', json.dumps(req), allow_redirects=True, timeout=0.01)
        except requests.exceptions.ReadTimeout: 
            pass
        return None
