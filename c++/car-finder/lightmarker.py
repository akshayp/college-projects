#              Light Marker Annotation
#
#   (c) Scott & Linda Wills         20 November 2009

import os, Image, ImageDraw

SeqDir = "./seqs/"
TrlDir = "./trials/"

Colors = ['pink', 'red', 'green', 'blue', 'orange', 'yellow', 'skyblue', 'purple']

def LightMarker() :
    """ This routine walks through each sequence in the seqs directory, parses
    the report.txt file in that sequence and annotates the images in the sequence
    with position and velocity information contained in the report.txt file. """
    DirList = os.listdir(".")
    if SeqDir[2:-1] not in DirList :
        print "sequence directory %s is missing" % (SeqDir)
    elif TrlDir[2:-1] not in DirList :
        print "trial directory %s is missing" % (TrlDir)
    else :
        Trials = os.listdir(TrlDir)
        for Seq in Trials :
            print "annotating sequence %s" % (Seq)
            Files = os.listdir(TrlDir + Seq)
            for File in Files :
                if File == 'report.txt' :
                    ReportFile = open(TrlDir + Seq + '/' + File, 'r')
                    FrameData = {}
                    for Line in ReportFile.readlines() :
                        if ':' in Line :
                            Keyword, Tokens = Line.split(':')
                            Keyword = Keyword.strip()
                            Tokens = Tokens[:-1]
                            if Keyword == 'Seq' :
                                SeqName = Tokens.strip()
                                SeqName = SeqName.strip('"')
                                if SeqName <> Seq :
                                    print "Error: Sequence name %s does not match sequence directory %s" % (SeqName, Seq)
                            if Keyword == 'Start' :
                                Start = int(Tokens.strip())
                            elif Keyword == 'End' :
                                End = int(Tokens.strip())
                            elif Keyword == 'FN' :
                                FN = int(Tokens.strip())
                                FrameData[FN] = []
                            elif Keyword == 'CE' : # line is a data line
                                Tokens = Tokens.replace(')', ',')
                                Tokens = Tokens.replace('(', '')
                                Data = Tokens.split(',')
                                Xmin = int(Data[0].strip())
                                Ymin = int(Data[1].strip())
                                Xmax = int(Data[2].strip())
                                Ymax = int(Data[3].strip())
                                A = int(Data [4].strip())
                                FrameData[FN].append((Xmin, Ymin, Xmax, Ymax, A))
                            else : # do nothing
                                False
            for FN in range(Start, End+1) :
                Frame = Load_Frame(SeqDir + Seq, FN)
                Draw = ImageDraw.Draw(Frame)
                if FN in FrameData :
                    FN_Data = FrameData[FN]
                    for (Xmin, Ymin, Xmax, Ymax, A) in FN_Data :
                        Draw_Marker(FN, Draw, Xmin, Ymin, Xmax, Ymax)
                    Store_Frame(TrlDir + Seq, FN, Frame)
                else :
                    print "Error: Missing data for Frame %d" % (FN)
    
def Load_Frame(Seq, FN) :
    """ This routine loads a specified frame in a sequence and
    returns it. """
    Path = "%s/%05d.jpg" % (Seq, FN)
    Frame = Image.open(Path)
    return Frame

def Store_Frame(Seq, FN, Frame) :
    """ This routine saves an output frame. """
    Path = "%s/%05d.jpg" % (Seq, FN)
    Frame.save(Path, 'JPEG')

def Draw_Marker(FN, Draw, Xmin, Ymin, Xmax, Ymax, Width=1280, Height=720) :
    """ This routine draws a marker at the specfied position in the frame. """
    Y = (Ymax + Ymin) / 2
    L = Xmax - Xmin
    if L < Width / 5 :
        Color = "green"
    elif L < Width / 3 :
        Color = "yellow"
    else :
        Color = "red"
    Draw.line([(Xmin,Ymin),(Xmin,Ymax)], width=2, fill=Color)
    Draw.line([(Xmax,Ymin),(Xmax,Ymax)], width=2, fill=Color)
    Draw.line([(Xmin,Y),(Xmax,Y)], width=2, fill=Color)
              
if __name__ == '__main__' :
    LightMarker()
