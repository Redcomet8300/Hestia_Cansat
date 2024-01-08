from dash import dcc, html
from dash.dependencies import Input, Output
import plotly.graph_objs as go
import datetime
import serial
import dash
import os

AC_logo = 'assets/AClogo.png'
NSM_logo = 'assets/NSMlogo.png'
Hestia_logo = 'asset/Hestialogo.png'
SPAC_logo = 'asset/spacelogo.jpg'
Combine_logo = 'assets/Cansat For server.png'

app = dash.Dash(__name__)

app.layout = html.Div([
    html.Div([
        html.H1("Hestia Cansat", style={'display': 'inline-block', 'font-size': '0px'}),
    ], style={'marginLeft': '20px', 'marginTop': '20px'}),
    html.Div([
        html.Img(src=Combine_logo, style={'display':'inline-block'}),  # Adjust width and height as needed
    ], style={'marginRight': '20px', 'marginTop': '20px', 'textAlign': 'right'}),

    dcc.Graph(id='temp-graph', config={'displayModeBar': False}),
    dcc.Graph(id='pressure-graph', config={'displayModeBar': False}),
    dcc.Graph(id='xyz-graph', config={'displayModeBar': False}),
    dcc.Interval(
        id='graph-update',
        interval=3000,
        n_intervals=0
    ),
])

time_data = []
temp_data = []
pressure_data = []
x_data = []
y_data = []
z_data = []

ser = serial.Serial('COM4', 9600) # Setting port

def update_data():
    try:
        line = ser.readline().decode().strip()
        print(line)

        data_values = line.split(",")

        if len(data_values) == 5:
            new_time = datetime.datetime.now()
            try:
                new_temp, new_pressure, new_x, new_y, new_z = map(float, data_values)
            except ValueError as e:
                print(f"Error converting values to float: {e}")
                return

            time_data.append(new_time)
            temp_data.append(new_temp)
            pressure_data.append(new_pressure)
            x_data.append(new_x)
            y_data.append(new_y)
            z_data.append(new_z)

            if len(time_data) > 10:
                time_data.pop(0)
                temp_data.pop(0)
                pressure_data.pop(0)
                x_data.pop(0)
                y_data.pop(0)
                z_data.pop(0)
    except Exception as e:
        print(f"Error reading from serial port: {e}")

@app.callback([Output('temp-graph', 'figure'),
               Output('pressure-graph', 'figure'),
               Output('xyz-graph', 'figure')],
              [Input('graph-update', 'n_intervals')])
def update_graph(n):
    update_data()
    print(f"Temperature: {temp_data}")
    print(f"Pressure: {pressure_data}")
    print(f"X: {x_data}")
    print(f"Y: {y_data}")
    print(f"Z: {z_data}")

    temp_fig = go.Figure()
    temp_fig.add_trace(go.Scatter(x=time_data, y=temp_data, mode='lines+markers', name='Temperature'))
    temp_fig.update_layout(
        title='Temperature',
        xaxis=dict(title='Time', tickfont=dict(size=40)),
        yaxis=dict(title='Value', tickfont=dict(size=40)),
        title_font=dict(size=40),  
        xaxis_title_font=dict(size=40),  
        yaxis_title_font=dict(size=40), 
        height=800, 
    )

    pressure_fig = go.Figure()
    pressure_fig.add_trace(go.Scatter(x=time_data, y=pressure_data, mode='lines+markers', name='Pressure'))
    pressure_fig.update_layout(
        title='Pressure',
        xaxis=dict(title='Time', tickfont=dict(size=40)),
        yaxis=dict(title='Value', tickfont=dict(size=40)),
        title_font=dict(size=40),  
        xaxis_title_font=dict(size=40),  
        yaxis_title_font=dict(size=40), 
        height=800,
    )

    xyz_fig = go.Figure()
    xyz_fig.add_trace(go.Scatter(x=time_data, y=x_data, mode='lines+markers', name='X',))
    xyz_fig.add_trace(go.Scatter(x=time_data, y=y_data, mode='lines+markers', name='Y',))
    xyz_fig.add_trace(go.Scatter(x=time_data, y=z_data, mode='lines+markers', name='Z',))

    xyz_fig.update_layout(
        title='XYZ Values',
        xaxis=dict(title='Time', tickfont=dict(size=40)),
        yaxis=dict(title='Value', tickfont=dict(size=40)),
        title_font=dict(size=40),  
        xaxis_title_font=dict(size=40),  
        yaxis_title_font=dict(size=40), 
        height=800, 
    )

    return temp_fig, pressure_fig, xyz_fig

if __name__ == '__main__':
    app.run_server(debug=False)
