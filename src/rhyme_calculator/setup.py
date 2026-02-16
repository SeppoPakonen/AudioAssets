import os
from setuptools import setup, find_packages

setup(
    name="rhyme-calculator",
    version="0.1.0",
    description="A Python program that calculates rhyming scores for English lyrics",
    author="Your Name",
    author_email="your.email@example.com",
    packages=find_packages(),
    install_requires=[
        "pronouncing>=0.2.0",
        "click>=8.0.0"
    ],
    entry_points={
        'console_scripts': [
            'rhyme-calc=rhyme_calculator.cli:main',
        ],
    },
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved License",
        "Operating System :: OS Independent",
    ],
    python_requires='>=3.8',
)