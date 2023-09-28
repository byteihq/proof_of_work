import json
from time import time, sleep
from api_request import ApiRequest, ApiExceptions
from hashcash import HashCash


def main():
    try:
        for i in range(10):
            api = ApiRequest('http://localhost:8080', 'Moscow')
            response = api.send()
            print(response.content)
            challenge = json.loads(response.content)['HashChallenge']
            solution = HashCash(challenge).solve()
            print('time =', int(time()), '; solution =', solution)
            api.set_hash_cash(solution)
            response = api.send()
            print(response.content)
    except ApiExceptions as e:
        print(e)


if __name__ == '__main__':
    main()
