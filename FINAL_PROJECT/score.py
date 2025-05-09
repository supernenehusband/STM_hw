# FINAL_PROJECT/score.py

def save_score(score):
    """把新分數存進 scores.txt"""
    with open("scores.txt", "a") as file:
        file.write(f"{score}\n")

def get_scores():
    """讀取所有分數並回傳排序後的列表"""
    try:
        with open("scores.txt", "r") as file:
            scores = [int(line.strip()) for line in file]
        return sorted(scores, reverse=True)
    except FileNotFoundError:
        return []