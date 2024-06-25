import tkinter as tk
from tkinter import messagebox, scrolledtext
import subprocess
import os

class ProgmanApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Progman")
        self.root.configure(bg='yellow')

        self.prog_list = []

        self.text_area = scrolledtext.ScrolledText(root, width=40, height=15)
        self.text_area.pack(padx=10, pady=10)

        self.entry_text = tk.Entry(root, width=50)
        self.entry_text.pack(padx=10, pady=5)

        self.frame_buttons = tk.Frame(root, bg='yellow')
        self.frame_buttons.pack(padx=10, pady=5)

        self.button_add = tk.Button(self.frame_buttons, text="Add Program", command=self.add_program)
        self.button_add.grid(row=0, column=0, padx=5)

        self.button_exec = tk.Button(self.frame_buttons, text="Execute Program", command=self.execute_program)
        self.button_exec.grid(row=0, column=1, padx=5)

        self.button_show_time = tk.Button(self.frame_buttons, text="Show Time", command=self.show_time)
        self.button_show_time.grid(row=0, column=2, padx=5)

        self.load_progman_ini()

    def load_progman_ini(self):
        if os.path.exists("progman.ini"):
            with open("progman.ini", "r") as file:
                lines = file.readlines()
                self.prog_list = [line.strip() for line in lines]
                self.update_text_area()

    def save_progman_ini(self):
        with open("progman.ini", "w") as file:
            for program in self.prog_list:
                file.write(program + "\n")

    def update_text_area(self):
        self.text_area.delete(1.0, tk.END)
        for program in self.prog_list:
            self.text_area.insert(tk.END, program + "\n")

    def add_program(self):
        program = self.entry_text.get().strip()
        if program:
            self.prog_list.append(program)
            self.update_text_area()
            self.save_progman_ini()

    def execute_program(self):
        program = self.entry_text.get().strip()
        if program:
            try:
                subprocess.Popen(program, shell=True)
                messagebox.showinfo("Success", f"Program '{program}' started successfully.")
            except Exception as e:
                messagebox.showerror("Error", f"Failed to start program '{program}'.\nError: {str(e)}")

    def show_time(self):
        current_time = subprocess.check_output("date", shell=True).decode().strip()
        messagebox.showinfo("Current Time", current_time)

if __name__ == "__main__":
    root = tk.Tk()
    app = ProgmanApp(root)
    root.mainloop()

