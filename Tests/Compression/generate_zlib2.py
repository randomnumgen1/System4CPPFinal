import zlib

# A more complex string with varied characters and repetition
original_data = (b"The quick brown fox jumps over the lazy dog. " * 100) + (b"Pack my box with five dozen liquor jugs. " * 100)

# Compress the data at the highest level
compressed_data = zlib.compress(original_data, level=9)

# Write the compressed data to a file in the same directory
with open('max_compressed2.zlib', 'wb') as f:
    f.write(compressed_data)

print("Successfully generated 'max_compressed.zlib' with complex data.")
