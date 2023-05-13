import sys
import plotly.graph_objs as go
import numpy as np

path_file_txt = "C:/Users/ksksks/CLionProjects/untitled3/cmake-build-debug/results.txt"
sys.stdout = open(path_file_txt, "a")
file_txt = np.loadtxt(path_file_txt)
N = int(file_txt[0, 0])
X = float(file_txt[0, 1])
M = int(file_txt[0, 2])
Y = float(file_txt[0, 3])
K = int(file_txt[0, 4])
T_end = float(file_txt[0, 5])

x = np.linspace(0, X, N)
y = np.linspace(0, Y, M)
t = np.linspace(0, T_end, K)

fig = go.Figure([go.Heatmap(z=np.array(file_txt[1:(M + 1), 0:N]), colorbar=dict(title="T, °C"),
                            colorscale='viridis', x=x, y=y)])
frames = []
for i in range(1, K):
    frames.append(go.Frame(name=str(i), data=[go.Heatmap(z=np.array(file_txt[(i * M + 1):(M * i + M + 1), 0:N]),
                                                         colorbar=dict(title="T, °C"), colorscale='viridis',
                                                         x=x, y=y)]))
steps = []
for i in range(K):
    step = dict(
        label=str(round(t[i], 3)) + "c",
        method="animate",
        args=[[str(i)]],
    )
    steps.append(step)
sliders = [dict(
    steps=steps,
    len=0.9,
    x=0.1,
    currentvalue={"prefix": "Время: ", "font": {"size": 20}},
    pad={"b": 10, "t": 50})]
fig.update_layout(updatemenus=[dict(direction="left",
                                    pad={"r": 10, "t": 80},
                                    x=X*0.1,
                                    xanchor="right",
                                    y=0,
                                    yanchor="top",
                                    showactive=False,
                                    type="buttons",
                                    buttons=[dict(label="►", method="animate", args=[None, {"fromcurrent": True}]),
                                             dict(label="❚❚", method="animate", args=[[None],
                                                  {"frame": {"duration": 0, "redraw": False}, "mode": "immediate",
                                                   "transition": {"duration": 0}}])])]
                  )
fig.layout.sliders = sliders
fig.frames = frames
fig.update_layout(title="Temperature",
                  xaxis_title="X, m",
                  yaxis_title="Y, m",
                  margin=dict(l=50, r=50, t=50, b=50))

fig.write_html('first_figure.html', auto_open=True)
