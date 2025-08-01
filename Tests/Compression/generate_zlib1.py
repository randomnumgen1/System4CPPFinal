import zlib

# A long string of repeating characters to achieve high compression
original_data = b'A' * 100000

# Compress the data at the highest level
compressed_data = zlib.compress(original_data, level=9)

# Write the compressed data to a file
with open('max_compressed1.zlib', 'wb') as f:
    f.write(compressed_data)

print("Successfully generated 'max_compressed.zlib'")
