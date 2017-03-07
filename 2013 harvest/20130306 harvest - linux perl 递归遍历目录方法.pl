
Linux perl 遍历目录下文件的方法:


1、使用bash命令：
    
    #递归遍历目录下的所有c和h文件 应用该perl脚本
    $ du -a | awk -v IGNORECASE=1 'sub(/^[.0-9A-Z]+\s+/, ""){}; gsub(/ /, "\\ "){}; /\.((h)|(c))/' | xargs perl -i $0

    #perl 中代码：注意perl的 -i选项会让代码中的标准输出到当前文件中
    while(<>)
    {
        if(//)
        {
            print;
        }
    }

2、使用代码：
sub listdir { 
    my @arr, $j = 0;
    for($i=0;$i<=$#_;$i++) {
        if(-d $_[$i]) {
            if(opendir($handle, $_[$i])) {
                while($dir = readdir($handle)) {
                    if(!($dir =~ m/^\.$/) and !($dir =~ m/^(\.\.)$/)) {
                        print "$dir";
                        if(-d $_[$i]."\\$dir") {
                            $arr[$j++] = $_[$i]."\\$dir";
                            print "\t[DIR]"
                        }
                        print "\n";
                    }
                }
                closedir($handle);
            }
        }
    }
    if($j>0) {
        listdir (@arr);
    }
}
listdir 'D:\Fun\CS 1.6\platform\Steam';
