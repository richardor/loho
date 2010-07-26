目的：
    输出图片的大小信息，提供给美工

使用方法：
   1 将所有的gif图片放入一个目录下，假设该目录为pic_dir
   2 pic_dir 与生成的可执行程序get_gif_width_height位于同一目录， 执行命令./get_gif_width_height pic_dir 将会生成一个包括图片大小信息的文件picture_inf.txt
     该文件的格式如下， 
     gifname.gif -- w[555] h[426]

     左边为图片名， 右边为图片的尺寸信息

