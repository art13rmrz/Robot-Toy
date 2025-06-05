# -*- coding: utf-8 -*-
"""
Created on Wed Jun  4 23:13:22 2025

@author: art_r
"""

import os
import shutil

# Map folder keywords to standardized category names
categories = {
    "Thumbs Up": "Thumbs_Up",
    "Thumbs Down": "Thumbs_Down",
    "Right Swipe": "Right_Swipe",
    "Left Swipe": "Left_Swipe",
    "Stop Gesture": "Stop_Gesture"
}

def organize_and_rename_images():
    base_dir = os.path.dirname(os.path.abspath(__file__))
    dataset_dir = os.path.join(base_dir, "dataset")  # Folder containing all gesture folders

    # Prepare target folders and counters
    counters = {}
    for keyword, target_folder in categories.items():
        output_path = os.path.join(base_dir, target_folder)
        os.makedirs(output_path, exist_ok=True)
        counters[target_folder] = 1

    for folder_name in os.listdir(dataset_dir):
        folder_path = os.path.join(dataset_dir, folder_name)
        if not os.path.isdir(folder_path):
            continue

        for keyword, target_folder in categories.items():
            if keyword in folder_name:
                output_path = os.path.join(base_dir, target_folder)

                for file in os.listdir(folder_path):
                    if file.lower().endswith(('.jpg', '.jpeg', '.png', '.bmp', '.gif')):
                        ext = os.path.splitext(file)[1]
                        new_name = f"{target_folder}_{counters[target_folder]}{ext}"
                        counters[target_folder] += 1

                        src = os.path.join(folder_path, file)
                        dst = os.path.join(output_path, new_name)

                        shutil.copy2(src, dst)
                        print(f"Copied and renamed: {file} → {new_name}")
                break  # Match only one category per folder

# Run the function
organize_and_rename_images()
