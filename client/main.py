import json
from time import time, sleep
from api_request import ApiRequest, ApiExceptions
from hashcash import HashCash


def main():
    try:
        for i in range(100):
            api = ApiRequest('http://localhost:8080', 'Moscow')
            response = api.send()
            print(response.content)
            _json = json.loads(response.content)
            while 'HashChallenge' in _json:
                challenge = _json['HashChallenge']
                solution = HashCash(challenge).solve()
                print('time =', int(time()), '; solution =', solution)
                api.set_hash_cash(solution)
                response = api.send()
                print(response.content)
                _json = json.loads(response.content)

            sleep((i + 15)*10 / 1000)
    except ApiExceptions as e:
        print(e)


if __name__ == '__main__':
    main()
