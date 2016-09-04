DIR=toManufacturer
mkdir -p $DIR
rm -f $DIR/*
BOARD=CorboLight
echo "preparing production files for $BOARD"
#Files for top layer
cp sch-F.Cu.gtl $DIR/$BOARD.GTL #Copper
cp sch-F.Mask.gts $DIR/$BOARD.GTS #Solder mask
cp sch-F.SilkS.gto $DIR/$BOARD.GTO #Silk screen

#files for bottom layer
cp sch-B.Cu.gbl $DIR/$BOARD.GBL #Copper
cp sch-B.Mask.gbs $DIR/$BOARD.GBS #Solder mask
cp sch-B.SilkS.gbo $DIR/$BOARD.GBO #Silk screen

#Drill and outline
cp sch-Edge.Cuts.gm1 $DIR/$BOARD.GML #board outline
cp sch.drl $DIR/$BOARD.TXT #drill file

zip -r $BOARD.zip $DIR/*

echo "Done"
