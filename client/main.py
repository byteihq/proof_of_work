import json
from sys import argv
from time import time, sleep
from api_request import ApiRequest, ApiExceptions
from hashcash import HashCash


def main(wait_for_response: bool):
    total_requests = 0
    total_challenges = 0
    total_time = 0
    try:
        while True:
            api = ApiRequest('http://localhost:8080', wait_for_response, 'Moscow')
            response = api.send()
            total_requests += 1
            if response is None:
                continue
            print('Response:', response.content)
            _json = json.loads(response.content)
            while 'HashChallenge' in _json:
                total_challenges += 1
                challenge = _json['HashChallenge']
                print("New challenge:", challenge)
                challenge_solving_start = time()
                solution = HashCash(challenge).solve()
                challenge_solving_end = time()
                total_time += challenge_solving_end - challenge_solving_start
                print('Time spent on the solution:', challenge_solving_end - challenge_solving_start, 's. Solution: ', solution)
                api.set_hash_cash(solution)
                response = api.send()
                total_requests += 1
                if response is None:
                    continue
                print('Response:', response.content)
                _json = json.loads(response.content)
    except ApiExceptions as e:
        print(e)
    except KeyboardInterrupt:
        print(f'Statistics:\n\tTotal requests: {total_requests}\n\tTotal challenges: {total_challenges}')
        if total_challenges > 0:
            print(f'\tAverage time spending on the solution: {total_time / total_challenges}')

if __name__ == '__main__':
    main(('--no-wait' not in argv))
