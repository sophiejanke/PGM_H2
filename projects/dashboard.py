"""
    Dashboard for PRIMED Grid Modelling (in C++)
    Copyright 2023 (C)

    Behzad Hashemi PhD,
    email:  bhashemi@hilat.ca
    github: Behzad-h

    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

    CONTINUED USE OF THIS SOFTWARE CONSTITUTES ACCEPTANCE OF THESE TERMS.
"""

"""
    A web browser-based dashboard developed using Dash for visualizing the results of
    the PRIMED Grid Modelling projects.
"""




"""
    Required User Input  >>>
"""

# Specify the main folder path (project name)
main_folder_path = 'example_py'




"""
    Optional User Input  >>>
"""

# Specify the start date of the project
start_year = 2030




#====================================================================================
#======== Dashboard Code ============================================================

import dash
from dash import dcc, html
from dash.dependencies import Input, Output
import plotly.express as px
import pandas as pd
import os
import re

#====================================================================================
#====================================================================================

"""
    1. Reading the PGMcpp results
"""

"""
    1.1. Reading the csv files
"""

# Define the sub-folder paths
sub_folder_path_model = main_folder_path + '/Model'
sub_folder_path_combustion = main_folder_path + '/Production/Combustion'
sub_folder_path_noncombustion = main_folder_path + '/Production/Noncombustion'
sub_folder_path_renewable = main_folder_path + '/Production/Renewable'
sub_folder_path_storage = main_folder_path + '/Storage'


# Create empty dictionaries to store DataFrames
dataframes_model_main = {}
dataframes_combustion = {}
dataframes_noncombustion = {}
dataframes_renewable = {}
dataframes_storage = {}


# Read the 'time_series_results.csv' file of Model folder into a DataFrame and store
# it in the dictionary
dataframes_model_main['Model'] = pd.read_csv(os.path.join(sub_folder_path_model, 'time_series_results.csv')).iloc[:, 1:-1]

# Make a datetime series from 2019 and freq='1h' for one year
times = pd.date_range(start='2019-01-01 00:00:00', end='2019-12-31 23:00:00', freq='1h')

# Change the year part of the values in 'times' to 'start_year'
times = times.map(lambda x: x.replace(year=start_year))

# Extend 'times' based on the project lifetime
times = pd.concat([pd.Series(times + pd.DateOffset(years=i))
                    for i in range(round(len(dataframes_model_main['Model'])/8760))], ignore_index=True)


# Add the 'times' series to the 'Model' DataFrame
dataframes_model_main['Model']['datetime'] = times

# Add the operation modes columns to the 'Operation_Mode' DataFrame (these operation
# modes exist in PGMcpp-QEC Dispatch version)
try:
    dataframes_model_main['Operation_Mode'] = dataframes_model_main['Model'][['datetime',
                                                                              'Operation Mode A',
                                                                              'Operation Mode B']]
    dataframes_model_main['Model'].drop(['Net Load [kW]',
                                         'Operation Mode A',
                                         'Operation Mode B'], axis=1, inplace=True)
except:
    dataframes_model_main['Operation_Mode'] = pd.DataFrame(index=dataframes_model_main['Model'].index, 
                                                           columns=['datetime',
                                                                    'Operation Mode A',
                                                                    'Operation Mode B'])
    dataframes_model_main['Operation_Mode']['datetime'] = dataframes_model_main['Model']['datetime']
    dataframes_model_main['Operation_Mode'].loc[:, 'Operation Mode A'] = 0
    dataframes_model_main['Operation_Mode'].loc[:, 'Operation Mode B'] = 0
    dataframes_model_main['Model'].drop(['Net Load [kW]'], axis=1, inplace=True)


# Read the other 'time_series_results.csv' files into DataFrames and store them in 
# the respective dictionaries
folder_paths = [sub_folder_path_combustion, 
                sub_folder_path_noncombustion, 
                sub_folder_path_renewable, 
                sub_folder_path_storage]

cols_combustion_energy = cols_noncombustion_energy = cols_renewable_energy = ['datetime',
                                                                              'Production [kW]',
                                                                              'Dispatch [kW]',
                                                                              'Storage [kW]',
                                                                              'Curtailment [kW]']
cols_storage_energy = ['datetime',
                       'Charging Power [kW]',
                       'Discharging Power [kW]',
                       'Charge (at end of timestep) [kWh]',
                       'Sate of Health (at end of timestep) []']

for i, path in enumerate(folder_paths):
    path_selected = path
    try:
        # Iterate through each folder inside the main folder
        for folder in os.listdir(path_selected):
            folder_path = os.path.join(path_selected, folder)

            # Check if it's a directory
            if os.path.isdir(folder_path):
                # Look for the time_series_results.csv file inside each subfolder
                csv_file_path = os.path.join(folder_path, 'time_series_results.csv')
                
                # Check if the file exists
                if os.path.exists(csv_file_path):
                    # Read the CSV file into a DataFrame and store it in the dictionary
                    df = pd.read_csv(csv_file_path).iloc[:, :-1]
                    df['datetime'] = times

                    if path == sub_folder_path_combustion:
                        dataframes_combustion[folder] = df[cols_combustion_energy]
                    elif path == sub_folder_path_noncombustion:
                        dataframes_noncombustion[folder] = df[cols_noncombustion_energy]
                    elif path == sub_folder_path_renewable:
                        df = df.drop(df.columns[1], axis=1)
                        dataframes_renewable[folder] = df[cols_renewable_energy]
                    elif path == sub_folder_path_storage:
                        dataframes_storage[folder] = df
    except:
        pass

# Define the time column name
col_time = 'datetime'

#------------------------------------------------------------------------------------

"""
    1.2. Reading the .md file to extract the key parameters of the project
"""

# Specify the file path
file_path_model_summary = main_folder_path + '/Model/summary_results.md'

# Read the contents of the file
with open(file_path_model_summary, 'r') as file:
    file_contents_model_summary = file.read()

# Use regular expressions to find and extract the key values
# Extract project_lifetime
match = re.search(r'Years: (\S+)', file_contents_model_summary)
if match:
    project_lifetime = round(float(match.group(1)), 1)
else:
    project_lifetime = 0

# Extract total_net_present_cost
match = re.search(r'Net Present Cost: (\S+)', file_contents_model_summary)
if match:
    total_net_present_cost = float(match.group(1))
else:
    total_net_present_cost = 0

# Extract total_LCOE
match = re.search(r'Levellized Cost of Energy: (\S+)', file_contents_model_summary)
if match:
    total_LCOE = round(float(match.group(1)), 4)
else:
    total_LCOE = 0

# Extract total_renewable_fraction
match = re.search(r'Renewable Penetration: (\S+)', file_contents_model_summary)
if match:
    total_renewable_fraction = round(100 * float(match.group(1)), 2)
else:
    total_renewable_fraction = 0

# Extract total_dispatch_and_discharge
match = re.search(r" Discharge: (\S+)", file_contents_model_summary)
if match:
    total_dispatch_and_discharge = float(match.group(1))
else:
    total_dispatch_and_discharge = 0

# Extract total_fuel_consumed
match = re.search(r"Total Fuel Consumed: (\S+)", file_contents_model_summary)
if match:
    total_fuel_consumed = float(match.group(1))
else:
    total_fuel_consumed = 0

#------------------------------------------------------------------------------------

"""
    1.3. Reading the .md file to extract data for the Model Summary Tab
"""

pattern = re.compile(r'# Model Summary Results(.*?)## Results', re.DOTALL)
match = pattern.search(file_contents_model_summary)
if match:
    model_summary = match.group(1).strip()
else:
    model_summary = "No Model Summary!"

#------------------------------------------------------------------------------------

"""
    1.4. Reading the .md file to extract data for the Model Details Tab
"""

# Details of Combustion assets
specs_summary_combustion = {}

if os.path.isdir(sub_folder_path_combustion):
    # Iterate through folders
    for folder_name in os.listdir(sub_folder_path_combustion):
        folder_path = os.path.join(sub_folder_path_combustion, folder_name)

        # Check if the item is a directory
        if os.path.isdir(folder_path):
            # Find the summary_results.md file in each folder
            md_file_path = os.path.join(folder_path, "summary_results.md")

            # Check if the file exists
            if os.path.exists(md_file_path):
                # Read the contents of the .md file
                with open(md_file_path, 'r') as file:
                    file_contents = file.read()

                # Use regular expression to extract text between two texts
                pattern = re.compile(r'-------(.*?)## Results', re.DOTALL)
                match = pattern.search(file_contents)

                # Check if the match is found
                if match:
                    result_content = match.group(1).strip()
                    specs_summary_combustion[folder_name] = result_content
                else:
                    specs_summary_combustion[folder_name] = "No Specs Found!"

# Details of NonCombustion assets
specs_summary_noncombustion = {}

if os.path.isdir(sub_folder_path_noncombustion):
    # Iterate through folders
    for folder_name in os.listdir(sub_folder_path_noncombustion):
        folder_path = os.path.join(sub_folder_path_noncombustion, folder_name)

        # Check if the item is a directory
        if os.path.isdir(folder_path):
            # Find the summary_results.md file in each folder
            md_file_path = os.path.join(folder_path, "summary_results.md")

            # Check if the file exists
            if os.path.exists(md_file_path):
                # Read the contents of the .md file
                with open(md_file_path, 'r') as file:
                    file_contents = file.read()

                # Use regular expression to extract text between two texts
                pattern = re.compile(r'-------(.*?)## Results', re.DOTALL)
                match = pattern.search(file_contents)

                # Check if the match is found
                if match:
                    result_content = match.group(1).strip()
                    specs_summary_noncombustion[folder_name] = result_content
                else:
                    specs_summary_noncombustion[folder_name] = "No Specs Found!"


# Details of Renewable assets
specs_summary_renewable = {}

if os.path.isdir(sub_folder_path_renewable):
    # Iterate through folders
    for folder_name in os.listdir(sub_folder_path_renewable):
        folder_path = os.path.join(sub_folder_path_renewable, folder_name)

        # Check if the item is a directory
        if os.path.isdir(folder_path):
            # Find the summary_results.md file in each folder
            md_file_path = os.path.join(folder_path, "summary_results.md")

            # Check if the file exists
            if os.path.exists(md_file_path):
                # Read the contents of the .md file
                with open(md_file_path, 'r') as file:
                    file_contents = file.read()

                # Use regular expression to extract text between two texts
                pattern = re.compile(r'-------(.*?)## Results', re.DOTALL)
                match = pattern.search(file_contents)

                # Check if the match is found
                if match:
                    result_content = match.group(1).strip()
                    specs_summary_renewable[folder_name] = result_content
                else:
                    specs_summary_renewable[folder_name] = "No Specs Found!"


# Details of Storage assets
specs_summary_storage = {}

if os.path.isdir(sub_folder_path_storage):
    # Iterate through folders
    for folder_name in os.listdir(sub_folder_path_storage):
        folder_path = os.path.join(sub_folder_path_storage, folder_name)

        # Check if the item is a directory
        if os.path.isdir(folder_path):
            # Find the summary_results.md file in each folder
            md_file_path = os.path.join(folder_path, "summary_results.md")

            # Check if the file exists
            if os.path.exists(md_file_path):
                # Read the contents of the .md file
                with open(md_file_path, 'r') as file:
                    file_contents = file.read()

                # Use regular expression to extract text between two texts
                pattern = re.compile(r'-------(.*?)## Results', re.DOTALL)
                match = pattern.search(file_contents)

                # Check if the match is found
                if match:
                    result_content = match.group(1).strip()
                    specs_summary_storage[folder_name] = result_content
                else:
                    specs_summary_storage[folder_name] = "No Specs Found!"

#------------------------------------------------------------------------------------

"""
    1.5. Reading the .md file to extract data for the Results Summary Tab
"""

# Results summary for the general model
pattern = re.compile(r'## Results(.*?)Total Carbon Dioxide', re.DOTALL)
match = pattern.search(file_contents_model_summary)
if match:
    results_summary_model = match.group(1).strip()
else:
    results_summary_model = "No Results Summary!"


# Results summary for Combustion assets
results_summary_combustion = {}

if os.path.isdir(sub_folder_path_combustion):
    # Iterate through folders
    for folder_name in os.listdir(sub_folder_path_combustion):
        folder_path = os.path.join(sub_folder_path_combustion, folder_name)

        # Check if the item is a directory
        if os.path.isdir(folder_path):
            # Find the summary_results.md file in each folder
            md_file_path = os.path.join(folder_path, "summary_results.md")

            # Check if the file exists
            if os.path.exists(md_file_path):
                # Read the contents of the .md file
                with open(md_file_path, 'r') as file:
                    file_contents = file.read()

                # Use regular expression to extract text between two texts
                pattern = re.compile(r'## Results(.*?)Total Carbon Dioxide', re.DOTALL)
                match = pattern.search(file_contents)

                # Check if the match is found
                if match:
                    result_content = match.group(1).strip()
                    results_summary_combustion[folder_name] = result_content
                else:
                    results_summary_combustion[folder_name] = "No Results Found!"


# Results summary for NonCombustion assets
results_summary_noncombustion = {}

if os.path.isdir(sub_folder_path_noncombustion):
    # Iterate through folders
    for folder_name in os.listdir(sub_folder_path_noncombustion):
        folder_path = os.path.join(sub_folder_path_noncombustion, folder_name)

        # Check if the item is a directory
        if os.path.isdir(folder_path):
            # Find the summary_results.md file in each folder
            md_file_path = os.path.join(folder_path, "summary_results.md")

            # Check if the file exists
            if os.path.exists(md_file_path):
                # Read the contents of the .md file
                with open(md_file_path, 'r') as file:
                    file_contents = file.read()

                # Use regular expression to extract text between two texts
                pattern = re.compile(r'## Results(.*?)-----', re.DOTALL)
                match = pattern.search(file_contents)

                # Check if the match is found
                if match:
                    result_content = match.group(1).strip()
                    results_summary_noncombustion[folder_name] = result_content
                else:
                    results_summary_noncombustion[folder_name] = "No Results Found!"


# Results summary for Renewable assets
results_summary_renewable = {}

if os.path.isdir(sub_folder_path_renewable):
    # Iterate through folders
    for folder_name in os.listdir(sub_folder_path_renewable):
        folder_path = os.path.join(sub_folder_path_renewable, folder_name)

        # Check if the item is a directory
        if os.path.isdir(folder_path):
            # Find the summary_results.md file in each folder
            md_file_path = os.path.join(folder_path, "summary_results.md")

            # Check if the file exists
            if os.path.exists(md_file_path):
                # Read the contents of the .md file
                with open(md_file_path, 'r') as file:
                    file_contents = file.read()

                # Use regular expression to extract text between two texts
                pattern = re.compile(r'## Results(.*?)-----', re.DOTALL)
                match = pattern.search(file_contents)

                # Check if the match is found
                if match:
                    result_content = match.group(1).strip()
                    results_summary_renewable[folder_name] = result_content
                else:
                    results_summary_renewable[folder_name] = "No Results Found!"


# Results summary for Storage assets
results_summary_storage = {}

if os.path.isdir(sub_folder_path_storage):
    # Iterate through folders
    for folder_name in os.listdir(sub_folder_path_storage):
        folder_path = os.path.join(sub_folder_path_storage, folder_name)

        # Check if the item is a directory
        if os.path.isdir(folder_path):
            # Find the summary_results.md file in each folder
            md_file_path = os.path.join(folder_path, "summary_results.md")

            # Check if the file exists
            if os.path.exists(md_file_path):
                # Read the contents of the .md file
                with open(md_file_path, 'r') as file:
                    file_contents = file.read()

                # Use regular expression to extract text between two texts
                pattern = re.compile(r'## Results(.*?)-----', re.DOTALL)
                match = pattern.search(file_contents)

                # Check if the match is found
                if match:
                    result_content = match.group(1).strip()
                    results_summary_storage[folder_name] = result_content
                else:
                    results_summary_storage[folder_name] = "No Results Found!"

#------------------------------------------------------------------------------------

"""
    1.6. Analysing the Operation Modes
"""

# Make a dataframe with 'project_lifetime' rows and 2 columns named 'Mode A' and 
# 'Mode B', each row containing the summation of each operation mode for each year
df_operation_modes = dataframes_model_main['Operation_Mode'].drop(columns=['datetime']).groupby(
    dataframes_model_main['Operation_Mode'].index // 8760
    ).sum()

# Add the 'index'+1 of 'df_operation_modes' as a first column named 'Year'
df_operation_modes.insert(0, 'Year', df_operation_modes.index + 1)

# Change the name of columns 2 and 3 to 'Mode A (h)' and 'Mode B (h)'
df_operation_modes.columns = ['Year', 'Mode A (h)', 'Mode B (h)']

# Add two new columns named 'Mode A (%)' and 'Mode B (%)' as % of modes for each year
try:
    df_operation_modes['Mode A (%)'] = round(100 * df_operation_modes['Mode A (h)'] / 
                                             (df_operation_modes['Mode A (h)'] + df_operation_modes['Mode B (h)']), 1)
    df_operation_modes['Mode B (%)'] = round(100 * df_operation_modes['Mode B (h)'] / 
                                             (df_operation_modes['Mode A (h)'] + df_operation_modes['Mode B (h)']), 1)
except:
    df_operation_modes['Mode A (%)'] = 0
    df_operation_modes['Mode B (%)'] = 0

#------------------------------------------------------------------------------------

"""
    1.7. Reading the .md file to extract data for the Emissions Tab
"""

# Read the general emissions summary
pattern = re.compile(r'Total Carbon Dioxide.*?(?=\n\n|\Z)', re.DOTALL)
match = pattern.search(file_contents_model_summary)
if match:
    emission_summary_model = match.group().strip()
else:
    emission_summary_model = "No Emission Results!"


# Emissions summary for Combustion assets
emission_summary_combustion = {}

if os.path.isdir(sub_folder_path_combustion):
    # Iterate through folders
    for folder_name in os.listdir(sub_folder_path_combustion):
        folder_path = os.path.join(sub_folder_path_combustion, folder_name)

        # Check if the item is a directory
        if os.path.isdir(folder_path):
            # Find the summary_results.md file in each folder
            md_file_path = os.path.join(folder_path, "summary_results.md")

            # Check if the file exists
            if os.path.exists(md_file_path):
                # Read the contents of the .md file
                with open(md_file_path, 'r') as file:
                    file_contents = file.read()

                # Use regular expression to extract text between two texts
                pattern = re.compile(r'Total Carbon Dioxide.*?(?=\n\n|\Z)', re.DOTALL)
                match = pattern.search(file_contents)

                # Check if the match is found
                if match:
                    result_content = match.group().strip()
                    emission_summary_combustion[folder_name] = result_content
                else:
                    emission_summary_combustion[folder_name] = "No Results Found!"




#====================================================================================
#====================================================================================

"""
    2. Defining the Tabs contents
"""

# Tab 1
children_tab_1 = [
        dcc.Tab(label='Model Summary', value='tab-1', children=[
            html.Div([
                dcc.Markdown("**The overal model configuration is as follows:**", 
                             style={'font-size':'1.2em', 'width':'95%', 'margin':'auto', 'margin-bottom':'1px', 'font-weight':'bold'}),
            ], style={'width':'100%', 'text-align':'left', 'padding':'1px'}),
            html.Div([
                dcc.Markdown(f"{model_summary.replace('## ', '### ')}", 
                             style={'width':'80%', 'margin':'auto', 'border':'1px solid #ddd', 'padding':'10px'}),
            ], style={'width':'100%', 'text-align':'left', 'padding':'20px'}),
        ]),
]


# Tab 2
children_tab_2 = [
        dcc.Tab(label='Model Details', value='tab-2', children=[
            html.Div([
                dcc.Markdown("**The specifications of all the assets are as follows:**", 
                             style={'font-size':'1.2em', 'width':'95%', 'margin':'auto', 'margin-bottom':'1px', 'font-weight':'bold'}),
            ], style={'width':'100%', 'text-align':'left', 'padding':'1px'}),

            html.Div([
                dcc.Markdown("**Combustion Assets Specifications:**", 
                             style={'font-size':'1.5em', 'color':'blue', 'width':'80%', 'margin':'auto', 'margin-bottom':'1px', 'font-weight':'bold'}),
            ], style={'width':'100%', 'text-align':'left', 'padding':'1px'}),
            *[
                html.Div([
                    dcc.Markdown(f"### **{folder_name}**: <br><br>{result.replace('## ', '#### ')}", 
                                 style={'width':'80%', 'margin':'auto', 'text-align':'left', 'border':'1px solid #ddd', 'padding':'10px'}, 
                                 dangerously_allow_html=True),
                ], style={'width':'100%', 'text-align':'left', 'padding':'20px'})
                for folder_name, result in specs_summary_combustion.items()
            ],

            html.Div([
                dcc.Markdown("**NonCombustion Assets Specifications:**", 
                             style={'font-size':'1.5em', 'color':'blue', 'width':'80%', 'margin':'auto', 'margin-bottom':'1px', 'font-weight':'bold'}),
            ], style={'width':'100%', 'text-align':'left', 'padding':'1px'}),
            *[
                html.Div([
                    dcc.Markdown(f"### **{folder_name}**: <br><br>{result.replace('## ', '#### ')}", 
                                 style={'width':'80%', 'margin':'auto', 'text-align':'left', 'border':'1px solid #ddd', 'padding':'10px'}, 
                                 dangerously_allow_html=True),
                ], style={'width':'100%', 'text-align':'left', 'padding':'20px'})
                for folder_name, result in specs_summary_noncombustion.items()
            ],

            html.Div([
                dcc.Markdown("**Renewable Assets Specifications:**", 
                             style={'font-size':'1.5em', 'color':'blue', 'width':'80%', 'margin':'auto', 'margin-bottom':'1px', 'font-weight':'bold'}),
            ], style={'width':'100%', 'text-align':'left', 'padding':'1px'}),
            *[
                html.Div([
                    dcc.Markdown(f"### **{folder_name}**: <br><br>{result.replace('## ', '#### ')}", 
                                 style={'width':'80%', 'margin':'auto', 'text-align':'left', 'border':'1px solid #ddd', 'padding':'10px'}, 
                                 dangerously_allow_html=True),
                ], style={'width':'100%', 'text-align':'left', 'padding':'20px'})
                for folder_name, result in specs_summary_renewable.items()
            ],

            html.Div([
                dcc.Markdown("**Storage Assets Specifications:**", 
                             style={'font-size':'1.5em', 'color':'blue', 'width':'80%', 'margin':'auto', 'margin-bottom':'1px', 'font-weight':'bold'}),
            ], style={'width':'100%', 'text-align':'left', 'padding':'1px'}),
            *[
                html.Div([
                    dcc.Markdown(f"### **{folder_name}**: <br><br>{result.replace('## ', '#### ')}", 
                                 style={'width':'80%', 'margin':'auto', 'text-align':'left', 'border':'1px solid #ddd', 'padding':'10px'}, 
                                 dangerously_allow_html=True),
                ], style={'width':'100%', 'text-align':'left', 'padding':'20px'})
                for folder_name, result in specs_summary_storage.items()
            ],
        ]),
]

# Tab 3
children_tab_3 = [
        dcc.Tab(label='Results Summary', value='tab-3', children=[
            html.Div([
                dcc.Markdown("**System Level Results:**", 
                             style={'font-size':'1.5em', 'color':'blue', 'width':'80%', 'margin':'auto', 'margin-bottom':'1px', 'font-weight':'bold'}),
            ], style={'width':'100%', 'text-align':'left', 'padding':'1px'}),
            html.Div([
                dcc.Markdown(f"{results_summary_model}", 
                             style={'width':'80%', 'margin':'auto', 'border':'1px solid #ddd', 'padding':'10px'}),
            ], style={'width':'100%', 'text-align':'left', 'padding':'20px'}),

            html.Div([
                dcc.Markdown("**Combustion Assets Results:**", 
                             style={'font-size':'1.5em', 'color':'blue', 'width':'80%', 'margin':'auto', 'margin-bottom':'1px', 'font-weight':'bold'}),
            ], style={'width':'100%', 'text-align':'left', 'padding':'1px'}),
            *[
                html.Div([
                    dcc.Markdown(f"**{folder_name}**: <br><br>{result}", 
                                 style={'width':'80%', 'margin':'auto', 'text-align':'left', 'border':'1px solid #ddd', 'padding':'10px'}, 
                                 dangerously_allow_html=True),
                ], style={'width':'100%', 'text-align':'left', 'padding':'20px'})
                for folder_name, result in results_summary_combustion.items()
            ],

            html.Div([
                dcc.Markdown("**NonCombustion Assets Results:**", 
                             style={'font-size':'1.5em', 'color':'blue', 'width':'80%', 'margin':'auto', 'margin-bottom':'1px', 'font-weight':'bold'}),
            ], style={'width':'100%', 'text-align':'left', 'padding':'1px'}),
            *[
                html.Div([
                    dcc.Markdown(f"**{folder_name}**: <br><br>{result}", 
                                 style={'width':'80%', 'margin':'auto', 'text-align':'left', 'border':'1px solid #ddd', 'padding':'10px'}, 
                                 dangerously_allow_html=True),
                ], style={'width':'100%', 'text-align':'left', 'padding':'20px'})
                for folder_name, result in results_summary_noncombustion.items()
            ],

            html.Div([
                dcc.Markdown("**Renewable Assets Results:**", 
                             style={'font-size':'1.5em', 'color':'blue', 'width':'80%', 'margin':'auto', 'margin-bottom':'1px', 'font-weight':'bold'}),
            ], style={'width':'100%', 'text-align':'left', 'padding':'1px'}),
            *[
                html.Div([
                    dcc.Markdown(f"**{folder_name}**: <br><br>{result}", 
                                 style={'width':'80%', 'margin':'auto', 'text-align':'left', 'border':'1px solid #ddd', 'padding':'10px'}, 
                                 dangerously_allow_html=True),
                ], style={'width':'100%', 'text-align':'left', 'padding':'20px'})
                for folder_name, result in results_summary_renewable.items()
            ],

            html.Div([
                dcc.Markdown("**Storage Assets Results:**", 
                             style={'font-size':'1.5em', 'color':'blue', 'width':'80%', 'margin':'auto', 'margin-bottom':'1px', 'font-weight':'bold'}),
            ], style={'width':'100%', 'text-align':'left', 'padding':'1px'}),
            *[
                html.Div([
                    dcc.Markdown(f"**{folder_name}**: <br><br>{result}", 
                                 style={'width':'80%', 'margin':'auto', 'text-align':'left', 'border':'1px solid #ddd', 'padding':'10px'}, 
                                 dangerously_allow_html=True),
                ], style={'width':'100%', 'text-align':'left', 'padding':'20px'})
                for folder_name, result in results_summary_storage.items()
            ],

            html.Div([
                dcc.Markdown("**Operation Modes:**", 
                             style={'font-size':'1.5em', 'color':'blue', 'width':'80%', 'margin':'auto', 'margin-bottom':'1px', 'font-weight':'bold'}),
            ], style={'width':'100%', 'text-align':'left', 'padding':'1px'}),
            *[
                html.Table([
                    *[html.Tr([html.Th(col)] + [html.Td(df_operation_modes[col][i], 
                                                        style={'border':'1px solid #ddd'}) for i in range(len(df_operation_modes))]) 
                                                        for col in df_operation_modes[['Year','Mode A (h)','Mode B (h)']].columns]
                ], style={'width':'auto', 'margin':'auto', 'border':'1px solid #ddd', 'padding':'30px'})
            ],
            *[
                html.Table([
                    *[html.Tr([html.Th(col)] + [html.Td(df_operation_modes[col][i], 
                                                        style={'border':'1px solid #ddd'}) for i in range(len(df_operation_modes))]) 
                                                        for col in df_operation_modes[['Year','Mode A (%)','Mode B (%)']].columns]
                ], style={'width':'auto', 'margin':'auto', 'border':'1px solid #ddd', 'padding':'30px'})
            ],
        ]),
]

# Tab 4
children_tab_4 = [
        dcc.Tab(label='Overal Dispatch', value='tab-4', children=[
            html.Div([
                dcc.Markdown("Slider:", 
                             style={'font-size':'1.2em', 'width':'95%', 'margin':'auto', 'margin-bottom':'1px'}),
            ], style={'width':'100%', 'text-align':'left', 'padding':'1px'}),

            dcc.RangeSlider(
                id='x-axis-slider',
                min=0,
                max=8760,
                step=1,
                marks={i: str(i) for i in range(0, 8760+1, round(8760/1750)*100)},
                value=[0, 8760],
                allowCross=True,
            ),

            *[dcc.Graph(id=f'model-line-plot-1', style={'height': '70vh'})], 
            *[dcc.Graph(id=f'model-line-plot-2', style={'height': '40vh','width': '171vh'})], 
        ]),
]

# Tab 5
if os.path.isdir(sub_folder_path_combustion):
    children_tab_5 = [
        dcc.Tab(label='Combustions', value='tab-5', children=[
                dcc.Dropdown(
                    id='column-selector-combustion-plot',
                    options=[{'label': col, 'value': col} 
                             for col in dataframes_combustion[list(dataframes_combustion.keys())[0]].drop(columns=['datetime']).columns[:]],
                    multi=True,
                    value=[dataframes_combustion[list(dataframes_combustion.keys())[0]].columns[1]],
                    style={'width': '48%', 'margin-bottom': '20px'},
                ),
                *[dcc.Graph(id=f'combustion-line-plot-{i}') for i in range(len(dataframes_combustion))],              
        ]),
    ]
else:
    children_tab_5 = []


# Tab 6
if os.path.isdir(sub_folder_path_noncombustion):
    children_tab_6 = [
        dcc.Tab(label='NonCombustions', value='tab-6', children=[
                dcc.Dropdown(
                    id='column-selector-noncombustion-plot',
                    options=[{'label': col, 'value': col} 
                             for col in dataframes_noncombustion[list(dataframes_noncombustion.keys())[0]].drop(columns=['datetime']).columns[:]],
                    multi=True,
                    value=[dataframes_noncombustion[list(dataframes_noncombustion.keys())[0]].columns[1]],
                    style={'width': '48%', 'margin-bottom': '20px'},
                ),
                *[dcc.Graph(id=f'noncombustion-line-plot-{i}') for i in range(len(dataframes_noncombustion))],    
        ]),
    ]
else:
    children_tab_6 = []


# Tab 7
if os.path.isdir(sub_folder_path_renewable):
    children_tab_7 = [
        dcc.Tab(label='Renewables', value='tab-7', children=[
            dcc.Dropdown(
                id='column-selector-renewable-plot',
                options=[{'label': col, 'value': col} 
                         for col in dataframes_renewable[list(dataframes_renewable.keys())[0]].drop(columns=['datetime']).columns[:]],
                multi=True,
                value=[dataframes_renewable[list(dataframes_renewable.keys())[0]].columns[1]],
                style={'width': '48%', 'margin-bottom': '20px'},
            ),
            *[dcc.Graph(id=f'renewable-line-plot-{i}') for i in range(len(dataframes_renewable))],  
        ]),
    ]
else:
    children_tab_7 = []

# Tab 8
if os.path.isdir(sub_folder_path_storage):
    children_tab_8 = [
        dcc.Tab(label='Storages', value='tab-8', children=[
            dcc.Dropdown(
                id='column-selector-storage-power-plot',
                options=[{'label': col, 'value': col} 
                         for col in dataframes_storage[list(dataframes_storage.keys())[0]].drop(columns=['datetime']).columns[1:4]],
                multi=True,
                value=[dataframes_storage[list(dataframes_storage.keys())[0]].columns[1]],
                style={'width': '48%', 'margin-bottom': '20px'},
            ),
            *[dcc.Graph(id=f'storage-power-line-plot-{i}') for i in range(len(dataframes_storage))],  
            *[dcc.Graph(id=f'storage-health-line-plot-{i}', style={'width': '171vh'}) for i in range(len(dataframes_storage))],
        ]),
    ]
else:
    children_tab_8 = []

# Tab 9
children_tab_9 = [
        # Tab 9
        dcc.Tab(label='Emissions', value='tab-9', children=[
            html.Div([
                dcc.Markdown("**System Level Emissions:**", 
                             style={'font-size':'1.5em', 'color':'blue', 'width':'80%', 'margin':'auto', 'margin-bottom':'1px', 'font-weight':'bold'}),
            ], style={'width':'100%', 'text-align':'left', 'padding':'1px'}),
            html.Div([
                dcc.Markdown(f"{emission_summary_model}", 
                             style={'width':'80%', 'margin':'auto', 'border':'1px solid #ddd', 'padding':'10px'}),
            ], style={'width':'100%', 'text-align':'left', 'padding':'20px'}),

            html.Div([
                dcc.Markdown("**Combustion Assets' Emissions:**", 
                             style={'font-size':'1.5em', 'color':'blue', 'width':'80%', 'margin':'auto', 'margin-bottom':'1px', 'font-weight':'bold'}),
            ], style={'width':'100%', 'text-align':'left', 'padding':'1px'}),
            *[
                html.Div([
                    dcc.Markdown(f"**{folder_name}**: <br><br>{result}", 
                                 style={'width':'80%', 'margin':'auto', 'text-align':'left', 'border':'1px solid #ddd', 'padding':'10px'}, 
                                 dangerously_allow_html=True),
                ], style={'width':'100%', 'text-align':'left', 'padding':'20px'})
                for folder_name, result in emission_summary_combustion.items()
            ],
        ]),
]


# Combine the children lists for both tabs
children = (children_tab_1 + 
            children_tab_2 + 
            children_tab_3 +
            children_tab_4 + 
            children_tab_5 + 
            children_tab_6 + 
            children_tab_7 + 
            children_tab_8 + 
            children_tab_9)


#====================================================================================
#====================================================================================

"""
    3. Initializing the Dash app
"""

# Initialize the Dash app
app = dash.Dash(__name__)

# Define the layout of the app
app.layout = html.Div([

    # Top header of the dashboard
    html.Div([
        
        # To show the project name (name of the folder containing the results)
        html.Div([
            html.H1("PGMcpp Results", style={'font-size':'2em', 'margin-bottom':'15px'}),
            html.Div(html.Strong("Project Name:"), style={'font-size': '1em'}),
            html.Div(f"{main_folder_path}", style={'font-size': '1em'}),
        ], style={'background-color':'#101D6B', 'padding':'40px', 'color':'white', 'flex':'50%'}),
    
        # To show the system architecture
        html.Div([
            html.Div(html.Strong("System Architecture:"), style={'font-size':'1em'}),
            html.Ul([
                html.Li(key, style={'margin-bottom': '3px'}) for dictionary in [dataframes_combustion, 
                                                                                dataframes_noncombustion, 
                                                                                dataframes_renewable, 
                                                                                dataframes_storage] if dictionary for key in dictionary.keys()
            ], style={'font-size':'0.9em'}),
        ], style={'background-color':'#101D6B', 'color':'white', 'flex':'30%', 'text-align':'left', 'padding-right':'40px', 'padding-top':'20px'}),

        # To show a summary of the results
        html.Div([
            html.Div(html.Strong("Main Summary:"), style={'font-size': '1em'}),
            html.Ul([
                html.Li(f"Project Lifetime: {project_lifetime} years", style={'margin-bottom': '10px'}),
                html.Li(f"Total NPC: {total_net_present_cost:,} $", style={'margin-bottom': '10px'}),
                html.Li(f"Levelized COE: {total_LCOE} $/kWh", style={'margin-bottom': '10px'}),
                html.Li(f"Renewable Fraction: {total_renewable_fraction} %", style={'margin-bottom': '10px'}),
                html.Li(f"Total Dispatch & Discharge: {total_dispatch_and_discharge:,} kWh", style={'margin-bottom': '10px'}),
                html.Li(f"Total Fuel Consumed: {total_fuel_consumed:,} L", style={'margin-bottom': '10px'}),
            ], style={'font-size': '0.9em'}),
        ], style={'background-color':'#101D6B', 'color':'white', 'flex':'40%', 'text-align':'left', 'padding-right':'40px', 'padding-top':'20px'}),

        # To show the year selection dropdown
        html.Div([
            html.Div(html.Strong("Year:"), style={'font-size': '1em', 'color': 'white', 'margin-bottom': '20px'}),
            # add a dropdown to select the year from 1 to 10 with 
            dcc.Dropdown(
                id='year-dropdown',
                options=[{'label': f'{i+1}', 'value': i} for i in range(int(project_lifetime))],
                value=0,
                style={'width':'100%', 'margin-bottom':'20px', 'color':'black'},
            ),
        ], style={'background-color':'#101D6B', 'flex':'10%', 'text-align':'left', 'padding-right':'40px', 'padding-top':'20px'}),

    ], style={'display':'flex', 'justify-content':'space-between','font-family':'Lato, sans-serif'}),

    # Tabs to show the contents of the dashboard
    dcc.Tabs(id='tabs', value='tab-all', children=children),
    
    html.Div(id='tabs-content'),

    # Footer of the dashboard
    html.Div([
        html.H1("Dashboard Designed for PRIMED Grid Modelling Program", 
                style={'font-size':'1em', 'margin-bottom':'20px'}),
    ], style={'background-color': '#101D6B', 'padding': '10px', 'color': 'white'}),

])


#====================================================================================
#====================================================================================

"""
    4. Defining callbacks to update the contents of plots
"""

@app.callback(
    Output('tabs-content', 'children'),
    [Input('tabs', 'value')]
)
def update_tabs_content(selected_tab):
    if selected_tab == 'tab-1':
        return ''
    elif selected_tab == 'tab-2':
        return ''
    elif selected_tab == 'tab-3':
        return ''
    elif selected_tab == 'tab-4':
        return ''
    elif selected_tab == 'tab-5':
        return ''
    elif selected_tab == 'tab-6':
        return ''
    elif selected_tab == 'tab-7':
        return ''
    elif selected_tab == 'tab-8':
        return ''
    elif selected_tab == 'tab-9':
        return ''
    

# Callback to update plots based on dropdown values and x-axis slider of Model tab

# Load and Dispatch plot
@app.callback(
    [Output(f'model-line-plot-1', 'figure')],
    [Input('x-axis-slider', 'value'),
     Input('year-dropdown', 'value')]
)
def update_model_plots(selected_x_range_model, selected_year):
    figs = []
    filtered_df = dataframes_model_main['Model'][selected_year*8760:
                                                 (selected_year+1)*8760].iloc[selected_x_range_model[0]:
                                                                              selected_x_range_model[1] + 1, :]
    fig = px.line(filtered_df, x=col_time, y=filtered_df.columns, title='Load & Dispatch', labels={col_time: 'Time'})
    fig.update_layout(legend_title=f'Legend 1')
    figs.append(fig)
    return figs

# Operation Modes plot
@app.callback(
    [Output(f'model-line-plot-2', 'figure')],
    [Input('x-axis-slider', 'value'),
     Input('year-dropdown', 'value')]
)
def update_model_plots(selected_x_range_model, selected_year):
    figs = []
    filtered_df = dataframes_model_main['Operation_Mode'][selected_year*8760:
                                                          (selected_year+1)*8760].iloc[selected_x_range_model[0]:
                                                                                       selected_x_range_model[1] + 1, :]
    fig = px.line(filtered_df, x=col_time, y=filtered_df.columns, title='Operation Modes', labels={col_time: 'Time'})
    fig.update_layout(legend_title=f'Legend 2')
    figs.append(fig)
    return figs

# Combustion tab
if os.path.isdir(sub_folder_path_combustion):
    @app.callback(
        [Output(f'combustion-line-plot-{i}', 'figure') for i in range(len(dataframes_combustion))],
        [Input('column-selector-combustion-plot', 'value'),
        Input('x-axis-slider', 'value'),
        Input('year-dropdown', 'value')]
    )
    def update_combustion_plots(selected_columns_plot, selected_x_range, selected_year):
        figs = []
        for i, (name, df) in enumerate(dataframes_combustion.items()):
            filtered_df = df[selected_year*8760:
                             (selected_year+1)*8760].iloc[selected_x_range[0]:
                                                          selected_x_range[1] + 1, :]
            fig = px.line(filtered_df, x=col_time, y=selected_columns_plot, title=name, labels={col_time: 'Time'}, render_mode="SVG")
            fig.update_layout(legend_title=f'Legend {i+1}')
            figs.append(fig)
        return figs
    
# NonCombustion tab
if os.path.isdir(sub_folder_path_noncombustion):
    @app.callback(
        [Output(f'noncombustion-line-plot-{i}', 'figure') for i in range(len(dataframes_noncombustion))],
        [Input('column-selector-noncombustion-plot', 'value'),
        Input('x-axis-slider', 'value'),
        Input('year-dropdown', 'value')]
    )
    def update_noncombustion_plots(selected_columns_plot, selected_x_range, selected_year):
        figs = []
        for i, (name, df) in enumerate(dataframes_noncombustion.items()):
            filtered_df = df[selected_year*8760:
                             (selected_year+1)*8760].iloc[selected_x_range[0]:
                                                          selected_x_range[1] + 1, :]
            fig = px.line(filtered_df, x=col_time, y=selected_columns_plot, title=name, labels={col_time: 'Time'})
            fig.update_layout(legend_title=f'Legend {i+1}')
            figs.append(fig)
        return figs

# Renewable tab
if os.path.isdir(sub_folder_path_renewable):
    @app.callback(
        [Output(f'renewable-line-plot-{i}', 'figure') for i in range(len(dataframes_renewable))],
        [Input('column-selector-renewable-plot', 'value'),
        Input('x-axis-slider', 'value'),
        Input('year-dropdown', 'value')]
    )
    def update_renewable_plots(selected_columns_renewable_plot, selected_x_range_renewable, selected_year):
        figs = []
        for i, (name, df) in enumerate(dataframes_renewable.items()):
            filtered_df = df[selected_year*8760:
                             (selected_year+1)*8760].iloc[selected_x_range_renewable[0]:
                                                          selected_x_range_renewable[1] + 1, :]
            fig = px.line(filtered_df, x=col_time, y=selected_columns_renewable_plot, title=name, labels={col_time: 'Time'})
            fig.update_layout(legend_title=f'Legend {i+1}')
            figs.append(fig)
        return figs

# Storage tab
if os.path.isdir(sub_folder_path_storage):
    # Power plot
    @app.callback(
        [Output(f'storage-power-line-plot-{i}', 'figure') for i in range(len(dataframes_storage))],
        [Input('column-selector-storage-power-plot', 'value'),
        Input('x-axis-slider', 'value'),
        Input('year-dropdown', 'value')]
    )
    def update_storage_power_plots(selected_columns_storage_plot, selected_x_range_storage, selected_year):
        figs = []
        for i, (name, df) in enumerate(dataframes_storage.items()):
            filtered_df = df[selected_year*8760:
                             (selected_year+1)*8760].iloc[selected_x_range_storage[0]:
                                                          selected_x_range_storage[1] + 1, :]
            fig = px.line(filtered_df, x=col_time, y=selected_columns_storage_plot, title=name, labels={col_time: 'Time'})
            fig.update_layout(legend_title=f'Legend {i+1}')
            figs.append(fig)
        return figs
    
    # Health plot
    @app.callback(
        [Output(f'storage-health-line-plot-{i}', 'figure') for i in range(len(dataframes_storage))],
        [Input('column-selector-storage-power-plot', 'value'),
        Input('x-axis-slider', 'value'),
        Input('year-dropdown', 'value')]
    )
    def update_storage_health_plots(selected_columns_storage_plot, selected_x_range_storage, selected_year):
        figs = []
        for i, (name, df) in enumerate(dataframes_storage.items()):
            filtered_df = df[selected_year*8760:
                             (selected_year+1)*8760].iloc[selected_x_range_storage[0]:
                                                          selected_x_range_storage[1] + 1, :]
            fig = px.line(filtered_df, x=col_time, y='State of Health (at end of timestep) [ ]', title=name, labels={col_time: 'Time'})
            fig.update_layout(legend_title=f'Legend {i+1}')
            figs.append(fig)

        return figs


#====================================================================================
#====================================================================================

"""
    5. Running the app
"""

if __name__ == '__main__':
    app.run_server(debug=True)


