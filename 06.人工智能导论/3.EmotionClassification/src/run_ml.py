''' Traditional ml methods, including naive bayes, svm and random forest'''
import argparse

import numpy as np
from scipy.stats import pearsonr
from sklearn.ensemble import RandomForestClassifier
from sklearn.feature_extraction.text import CountVectorizer, TfidfTransformer
from sklearn.linear_model import SGDClassifier
from sklearn.metrics import classification_report
from sklearn.naive_bayes import MultinomialNB
from sklearn.pipeline import Pipeline

from dataset import class_list, test_path, train_path


def get_data(input_path):
    contents, labels, votes = [], [], []
    with open(input_path, 'r', encoding='utf8') as f:
        for line in f.readlines():
            content, label, vote = line.strip().split('\t')
            contents.append(content)
            labels.append(int(label))
            votes.append(list(map(int, vote.split())))
    return (contents, labels, votes)


def linear_SVM():
    return Pipeline([
        ('count_vect', CountVectorizer(ngram_range=(1, 2))),
        ('tfidf', TfidfTransformer()),
        ('clf', SGDClassifier(loss="hinge", alpha=1e-3, random_state=1))
    ])


def MNB_Classifier():
    return Pipeline([
        ('count_vec', CountVectorizer()),
        ('clf', MultinomialNB())
    ])


def Random_Forest():
    return Pipeline([
        ('count_vect', CountVectorizer()),
        ('tfidf', TfidfTransformer()),
        ('clf', RandomForestClassifier(n_estimators=1000, random_state=1, n_jobs=-1))
    ])


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--method", type=str, default="naive_bayes",
                        help="One of svm, random_forest and naive_bayes.")
    args = parser.parse_args()

    if args.method == "naive_bayes":
        model = MNB_Classifier()
    elif args.method == "svm":
        model = linear_SVM()
    elif args.method == "random_forest":
        model = Random_Forest()
    else:
        raise ValueError

    train_data, train_labels, _ = get_data(train_path)
    test_data, test_labels, test_votes = get_data(test_path)

    model.fit(train_data, train_labels)
    print(f"Model: {args.method}",)

    if args.method == "svm":
        predicted = model.predict(test_data)
    else:
        test_votes = np.asarray(test_votes, dtype=float)
        pred_votes = model.predict_proba(test_data)
        predicted = np.argmax(pred_votes, axis=1)
        coef = 0
        for pred_vote, vote in zip(pred_votes, test_votes):
            coef += pearsonr(pred_vote, vote)[0]
        coef /= len(predicted)
        print(f'coef: {coef:.4f}')

    report = classification_report(test_labels, predicted,
                                   target_names=class_list,
                                   digits=4, zero_division=0)
    print(report)


if __name__ == "__main__":
    main()
