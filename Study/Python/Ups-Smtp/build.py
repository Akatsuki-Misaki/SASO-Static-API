# build.py
import PyInstaller.__main__

PyInstaller.__main__.run([
    'ups-test.py',  # Python脚本文件名
    '--onefile',       # 生成单个可执行文件
])
