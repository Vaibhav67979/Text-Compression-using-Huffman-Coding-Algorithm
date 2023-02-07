import tkinter as tk
from collections import defaultdict, Counter
import heapq


class HuffmanNode:
    def __init__(self, char, freq):
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq < other.freq


class HuffmanEncoder:
    def __init__(self, text):
        self.text = text
        self.heap = []
        self.codes = {}
        self.reverse_mapping = {}
        self.make_frequency_dict()
        self.make_heap()
        self.make_huffman_tree()
        self.make_codes()

    def make_frequency_dict(self):
        frequency = defaultdict(int)
        for char in self.text:
            frequency[char] += 1
        self.frequency = frequency

    def make_heap(self):
        for char, freq in self.frequency.items():
            node = HuffmanNode(char, freq)
            heapq.heappush(self.heap, node)

    def merge_nodes(self):
        while len(self.heap) > 1:
            node1 = heapq.heappop(self.heap)
            node2 = heapq.heappop(self.heap)
            merged = HuffmanNode(None, node1.freq + node2.freq)
            merged.left = node1
            merged.right = node2
            heapq.heappush(self.heap, merged)

    def make_huffman_tree(self):
        self.merge_nodes()
        self.huffman_tree = self.heap[0]

    def make_codes_helper(self, node, current_code):
        if node is None:
            return
        if node.char is not None:
            self.codes[node.char] = current_code
            self.reverse_mapping[current_code] = node.char
            return
        self.make_codes_helper(node.left, current_code + "0")
        self.make_codes_helper(node.right, current_code + "1")

    def make_codes(self):
        self.make_codes_helper(self.huffman_tree, "")

    def compress(self):
        compressed = ""
        for char in self.text:
            compressed += self.codes[char]
        return compressed

    def decompress(self, compressed):
        start = 0
        end = 1
        decoded = ""
        while end <= len(compressed):
            if compressed[start:end] in self.reverse_mapping:
                decoded += self.reverse_mapping[compressed[start:end]]
                start = end
            end += 1
        return decoded


class App:
    def __init__(self, root):
        self.root = root
        self.root.title("Text Compression using Huffman Coding")
        self.root.geometry("400x400")
        self.root.resizable(False, False)

        self.input_frame = tk.Frame(self.root)
        self.input_frame.pack(pady=20)
        self.input_label = tk.Label(self.input_frame, text="Enter Text:")
        self.input_label.pack(side="left")
        self.input_entry = tk.Entry(self.input_frame, width=50)
        self.input_entry.pack(side="left")

        self.compress_button = tk.Button(self.root, text="Compress", command=self.compress)
        self.compress_button.pack(pady=20)

        self.output_frame = tk.Frame(self.root)
        self.output_frame.pack(pady=20)
        self.output_label = tk.Label(self.output_frame, text="Compressed Text:")
        self.output_label.pack(side="left")
        self.output_text = tk.Text(self.output_frame, height=10, width=50)
        self.output_text.pack(side="left")

        self.decompress_button = tk.Button(self.root, text="Decompress", command=self.decompress)
        self.decompress_button.pack(pady=20)

        self.original_frame = tk.Frame(self.root)
        self.original_frame.pack(pady=20)
        self.original_label = tk.Label(self.original_frame, text="Original Text:")
        self.original_label.pack(side="left")
        self.original_text = tk.Text(self.original_frame, height=10, width=50)
        self.original_text.pack(side="left")

    def compress(self):
        text = self.input_entry.get()
        encoder = HuffmanEncoder(text)
        compressed = encoder.compress()
        self.output_text.delete("1.0", tk.END)
        self.output_text.insert(tk.END, compressed)

    def decompress(self):
        compressed = self.output_text.get("1.0", tk.END).strip()
        decoder = HuffmanEncoder(compressed)
        original = decoder.decompress(compressed)
        self.original_text.delete("1.0", tk.END)
        self.original_text.insert(tk.END, original)


if __name__ == "__main__":
    root = tk.Tk()
    app = App(root)
    root.mainloop()
