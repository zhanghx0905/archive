from phe import paillier

public_key, private_key = paillier.generate_paillier_keypair()

num_candidate = int(input("请设置候选者人数:"))
num_voter = int(input("请设置投票者人数:"))

candidate_votes = [list() for _ in range(num_candidate)]

for idx in range(1, num_voter + 1):
    print(f"-------- 请第{idx}位投票者为候选者投票 --------")
    choi = int(input(f"请选择候选者(1-{num_candidate}):"))
    for i in range(num_candidate):
        res = int(i + 1 == choi)
        candidate_votes[i].append(public_key.encrypt(res))

print("-------- 计票人开始计票 --------")
secrets = [sum(candidate_vote) for candidate_vote in candidate_votes]
for (idx, secret) in enumerate(secrets, 1):
    print(f"第{idx}位候选人得票数密文: {secret.ciphertext()}")

print("-------- 公布计票结果 --------")
ans = [private_key.decrypt(secret) for secret in secrets]
for (idx, val) in enumerate(ans, 1):
    print(f"第{idx}位候选人得票数: {val}")
selected = ans.index(max(ans)) + 1
print(f"第{selected}位候选者得票最多。")
