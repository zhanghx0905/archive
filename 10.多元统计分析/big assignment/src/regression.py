import graphviz
import numpy as np
import pandas as pd
from sklearn.ensemble import RandomForestRegressor
from sklearn.linear_model import LassoCV, LinearRegression, RidgeCV
from sklearn.metrics import mean_squared_error
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeRegressor, export_graphviz

np.set_printoptions(precision=4, suppress=True)

df = pd.read_csv('Admission_Predict.csv')
df.loc[:, 'CGPA'] = df.loc[:, 'CGPA']/10*4 # 转GPA为四分制

X = df.iloc[:, 1:-1]
y = df.iloc[:, -1]
feature_names = ["GRE", "TOEFL", "University" , "SOP", "LOR", "GPA", "Research"]

# random forest for generate Feature Importances
rf_cls = RandomForestRegressor(n_estimators=1000, n_jobs=-1).fit(X, y)
print("features importances:")
print(feature_names)
print(rf_cls.feature_importances_)

OLS_reg = LinearRegression().fit(X, y)
print(f"OLS coef: {OLS_reg.intercept_:.4f}, {OLS_reg.coef_}")
predict = OLS_reg.predict(X)
print(f"OLS MSE: {mean_squared_error(y, predict):.6f}")



# 测试各种方法的性能
X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=1, test_size=0.2)

# regression tree
tree_reg = DecisionTreeRegressor(max_depth=4).fit(X_train, y_train)
predict = tree_reg.predict(X_test)
print(f"tree MSE: {mean_squared_error(y_test, predict):.6f}")

# OLS
OLS_reg = LinearRegression().fit(X_train, y_train)
print(f"OLS coef: {OLS_reg.intercept_:.4f}, {OLS_reg.coef_}")

predict = OLS_reg.predict(X_test)
mse = mean_squared_error(y_test, predict)
print(f"MSE: {mse:.6f}")

# ridge
alphas_to_test = np.linspace(0.01, 1, 100)
Ridge_reg = RidgeCV(alphas=alphas_to_test, cv=10).fit(X_train, y_train)

print(f"Ridge coef: {Ridge_reg.intercept_:.4f}, {Ridge_reg.coef_}")
predict = Ridge_reg.predict(X_test)
print(f"MSE: {mean_squared_error(y_test, predict):.6f}")

# lasso
alphas_to_test = np.linspace(0.001, 0.01, 10)
LASSO_reg = LassoCV(alphas=alphas_to_test).fit(X_train, y_train)

print(f"LASSO coef: {LASSO_reg.intercept_:.4f}, {LASSO_reg.coef_}")
predict = LASSO_reg.predict(X_test)
print(f"MSE: {mean_squared_error(y_test, predict):.6f}")

# 导出graphviz dot文件用于决策树可视化
export_graphviz(tree_reg, 
                out_file="tree.dot", 
                feature_names=feature_names, 
                impurity=False,
                special_characters=True, 
                filled=True, rounded=True)