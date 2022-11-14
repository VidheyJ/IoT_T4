import numpy as np
from sklearn.linear_model import LinearRegression
import pandas as pd
df = pd.read_excel('Calibration.xlsx')
print(df)
original = df.loc[df["Orignal"]>0,"Orignal"]
#print("test")
our_data = df.loc[df["Raw"]>0,"Raw"]
#print(our_data)
new = original.values
new1 = our_data.values
x= np.array(new).reshape((-1),1)
y = np.array(new1)
#print(x)
#print(y)
model = LinearRegression().fit(x, y)
r_sq = model.score(x, y)
print(f"coefficient of determination: {r_sq}")
print(f"intercept: {model.intercept_}")
print(f"slope: {model.coef_}")
y_pred = model.intercept_ + model.coef_ * x
#print(f"predicted response:\n{y_pred}")

import matplotlib.pyplot as plt
xpoints = (x)
ypoints = (y)
y1points = (y_pred)

plt.figure(1)
plt.plot(ypoints, label = 'Sensor Data')
#plt.legend(['Sensor Data'],)
#plt.legend(['Reference Data'])
plt.plot(xpoints, label = 'Reference Data')
plt.plot(y1points, label = 'Calibrated Data')
#plt.legend(['Calibrated Data'])
plt.legend(ncol=3)
plt.ylabel('CO2 concentration in ppm')
ax = plt.gca()
ax.get_xaxis().set_visible(False)
plt.plot()

plt.show()
