import tkinter as tk
import tkinter.filedialog
import tkinter.messagebox
import cv2
import numpy as np
from PIL import Image, ImageTk


def compress_image(input_image_path, output_image_path, quality=50):
    original_image = cv2.imread(input_image_path)
    # Encode the image with specified quality
    result, encoded_image = cv2.imencode('.jpg', original_image, [cv2.IMWRITE_JPEG_QUALITY, quality])
    # Check if the compression was successful
    if result:
        with open(output_image_path, 'wb') as output_file:
            output_file.write(encoded_image)
            return True
    else:
        return False


class App:
    def __init__(self, master):
        self.master = master
        self.input_image_path = None
        self.output_image_path = None
        self.quality = 50

        self.select_input_image_button = tk.Button(self.master, text="Select Input Image",
                                                   command=self.select_input_image)
        self.select_input_image_button.pack()

        self.input_image_label = tk.Label(self.master)
        self.input_image_label.pack()

        self.select_output_image_button = tk.Button(self.master, text="Select Output Image",
                                                    command=self.select_output_image)
        self.select_output_image_button.pack()

        self.output_image_label = tk.Label(self.master)
        self.output_image_label.pack()

        self.quality_scale = tk.Scale(self.master, from_=0, to=100, orient="horizontal", label="Quality", resolution=1,
                                      command=self.update_quality)
        self.quality_scale.pack()
        self.quality_scale.set(50)

        self.compress_button = tk.Button(self.master, text="Compress", command=self.compress)
        self.compress_button.pack()

    def select_input_image(self):
        self.input_image_path = tk.filedialog.askopenfilename(title="Select Input Image")
        self.display_input_image()

    def select_output_image(self):
        self.output_image_path = tk.filedialog.asksaveasfilename(title="Select Output Image", defaultextension=".jpg")

    def update_quality(self, value):
        self.quality = int(value)

    def compress(self):
        if not self.input_image_path or not self.output_image_path:
            tk.messagebox.showerror("Error", "Input image and/or output image not selected.")
            return
        result = compress_image(self.input_image_path, self.output_image_path,self.quality)
        if result:
            tk.messagebox.showinfo("Success", "Image compressed successfully.")
            self.display_output_image()
        else:
            tk.messagebox.showerror("Error", "Failed to compress image.")

    def display_input_image(self):
        if self.input_image_path:
            input_image = Image.open(self.input_image_path)
            input_image = input_image.resize((300, 300), Image.ANTIALIAS)
            input_image = ImageTk.PhotoImage(input_image)
            self.input_image_label.config(image=input_image)
            self.input_image_label.image = input_image

    def display_output_image(self):
        if self.output_image_path:
            output_image = Image.open(self.output_image_path)
            output_image = output_image.resize((300, 300), Image.ANTIALIAS)
            output_image = ImageTk.PhotoImage(output_image)
            self.output_image_label.config(image=output_image)
            self.output_image_label.image = output_image


if __name__ == "__main__":
    root = tk.Tk()
    root.title('Image Compression')
    root.geometry('600x400+50+50')
    root.resizable(True, True)
    app = App(root)
    root.mainloop()
