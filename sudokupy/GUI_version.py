import tkinter as tk

def read_sudoku():
    sudoku_values = []
    for i in range(9):
        row = []
        for j in range(9):
            value = int(entry_vars[i][j].get())
            row.append(value)
        sudoku_values.append(row)
    print(sudoku_values)  # You can modify this to do something else with the values

# Create main window
root = tk.Tk()
root.title("Sudoku Reader")

# Create entry variables
entry_vars = []
for i in range(9):
    row = []
    for j in range(9):
        var = tk.StringVar()
        entry = tk.Entry(root, width=3, textvariable=var)
        entry.grid(row=i, column=j)
        row.append(var)
    entry_vars.append(row)

# Create button to read Sudoku
read_button = tk.Button(root, text="Read Sudoku", command=read_sudoku)
read_button.grid(row=9, columnspan=9)

# Run the application
root.mainloop()
