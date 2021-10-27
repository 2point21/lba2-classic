@f:
@cd \projet\lba2\sources\rapport
@del rapport.mem > NUL
@
@echo ----------------------------------------------------------------- 
@echo Please Wait, exploring LBA2 HQR Files ...
@
@REM  ******** ANIM.HQR
@
@echo ----------------------------------------------------------------- > rapport.mem
@
@viewhqr f:\projet\lba2\game\anim -m -b -lf:\projet\lba2\tools\perso\anim > anim.mem
@string -i -d5 "Biggest Entity" anim.mem >> rapport.mem
@string -i -v "Average Size" anim.mem >> rapport.mem
@del anim.mem > NUL
@echo ----------------------------------------------------------------- >> rapport.mem 
@                
@REM  ******** BODY.HQR
@
@viewhqr f:\projet\lba2\game\body -m -b -lf:\projet\lba2\tools\perso\body > body.mem
@string -i -d5 "Biggest Entity" body.mem >> rapport.mem
@string -i -v "Average Size" body.mem >> rapport.mem
@del body.mem > NUL
@echo ----------------------------------------------------------------- >> rapport.mem 
@
@REM  ******** FILE3D.HQR
@
@viewhqr f:\projet\lba2\game\file3d -m -b -lf:\projet\lba2\tools\perso\file3d > file3d.mem
@string -i -d5 "Biggest Entity" file3d.mem >> rapport.mem
@string -i -v "Average Size" file3d.mem >> rapport.mem
@del file3d.mem > NUL
@echo ----------------------------------------------------------------- >> rapport.mem
@
@REM  ******** OBJFIX.HQR
@
@viewhqr f:\projet\lba2\game\objfix -m -b -lf:\projet\lba2\tools\perso\objfix > objfix.mem
@string -i -d5 "Biggest Entity" objfix.mem >> rapport.mem
@string -i -v "Average Size" objfix.mem >> rapport.mem
@del objfix.mem > NUL
@echo ----------------------------------------------------------------- >> rapport.mem
@
@REM  ******** FLOW.HQR
@
@viewhqr f:\projet\lba2\game\flow -m -b -lf:\projet\lba2\datas\flow\flow > flow.mem
@string -i -d5 "Biggest Entity" flow.mem >> rapport.mem
@string -i -v "Average Size" flow.mem >> rapport.mem
@del flow.mem > NUL
@echo ----------------------------------------------------------------- >> rapport.mem
@
@REM  ******** IMPACT.HQR
@
@viewhqr f:\projet\lba2\game\impact -m -b -lf:\projet\lba2\tools\perso\impact > impact.mem
@string -i -d5 "Biggest Entity" impact.mem >> rapport.mem
@string -i -v "Average Size" impact.mem >> rapport.mem
@del impact.mem > NUL
@echo ----------------------------------------------------------------- >> rapport.mem
@
@REM  ******** ANIM3DS.HQR
@
@viewhqr f:\projet\lba2\game\anim3ds -m -b > anim3ds.mem
@string -i "Biggest Entity" anim3ds.mem >> rapport.mem
@string -i -v "Average Size" anim3ds.mem >> rapport.mem
@del anim3ds.mem > NUL
@echo ----------------------------------------------------------------- >> rapport.mem
@
@REM  ******** SPRITES.HQR
@
@viewhqr f:\projet\lba2\game\sprites -m -b > sprites.mem
@string -i "Biggest Entity" sprites.mem >> rapport.mem
@string -i -v "Average Size" sprites.mem >> rapport.mem
@del sprites.mem > NUL
@echo ----------------------------------------------------------------- >> rapport.mem
@
@REM  ******** SPRIRAW.HQR
@
@viewhqr f:\projet\lba2\game\spriraw -m -b > spriraw.mem
@string -i "Biggest Entity" spriraw.mem >> rapport.mem
@string -i -v "Average Size" spriraw.mem >> rapport.mem
@del spriraw.mem > NUL
@echo ----------------------------------------------------------------- >> rapport.mem
@
@REM  ******** TEXT.HQR
@
@viewhqr f:\projet\lba2\game\text -m -b -lf:\projet\lba2\text\text > text.mem
@string -i -d5 "Biggest Entity" text.mem >> rapport.mem
@string -i -v "Average Size" text.mem >> rapport.mem
@del text.mem > NUL
@echo ----------------------------------------------------------------- >> rapport.mem
@
@REM  ******** SAMPLES.HQR
@
@viewhqr f:\projet\lba2\game\samples -m -b -lf:\projet\lba2\samples\samples > samples.mem
@string -i -d5 "Biggest Entity" samples.mem >> rapport.mem
@string -i -v "Average Size" samples.mem >> rapport.mem
@del samples.mem > NUL
@echo ----------------------------------------------------------------- >> rapport.mem
@
@echo -----------------------------------------------------------------
@type rapport.mem
@echo -----------------------------------------------------------------
