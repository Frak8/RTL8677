#!/usr/bin/perl
require "pwd.pl" || die "Required pwd.pl not found";

# This perl script automatically generates the samba.idb file

&initpwd;
$curdir = $ENV{"PWD"};

# We don't want the files listed in .cvsignore in the source tree
open(IGNORES,"../../source/.cvsignore") || die "Unable to open .cvsignore file\n";
while (<IGNORES>) {
  chop;
  $ignores{$_}++;
}
close IGNORES;

# get the names of all the binary files to be installed
open(MAKEFILE,"Makefile") || die "Unable to open Makefile\n";
@makefile = <MAKEFILE>;
@sprogs = grep(/^SPROGS /,@makefile);
@progs1 = grep(/^PROGS1 /,@makefile);
@progs = grep(/^PROGS /,@makefile);
@scripts = grep(/^SCRIPTS /,@makefile);
@codepage = grep(/^CODEPAGELIST/,@makefile);
close MAKEFILE;

if (@sprogs) {
  @sprogs[0] =~ s/^.*\=//;
  @sprogs = split(' ',@sprogs[0]);
}
if (@progs) {
  @progs[0] =~ s/^.*\=//;
  @progs = split(' ',@progs[0]);
}
if (@progs1) {
  @progs1[0] =~ s/^.*\=//;
  @progs1 = split(' ',@progs1[0]);
}
if (@scripts) {
  @scripts[0] =~ s/^.*\=//;
  @scripts = split(' ',@scripts[0]);
}
if (@codepage) {
  @codepage[0] =~ s/^.*\=//;
  chdir '../../source';
  # if we have codepages we need to create them for the package
  system("chmod +x ./installcp.sh");
  system("./installcp.sh . ../packaging/SGI/codepages . @codepage[0]");
  chdir $curdir;
  @codepage = sort split(' ',@codepage[0]);
}

# add my local files to the list of binaries to install
@bins = sort (@sprogs,@progs,@progs1,@scripts,("findsmb","sambalp","smbprint"));

# get a complete list of all files in the tree
chdir '../../';
&dodir('.');
chdir $curdir;

# the files installed in docs include all the original files in docs plus all
# the "*.doc" files from the source tree
@docs = sort byfilename grep (!/^docs\/$/ & (/^source\/.*\.doc$/ | /^docs\//),@allfiles);

@catman = sort grep(/^packaging\/SGI\/catman/ & !/\/$/, @allfiles);
@catman = sort bydirnum @catman;

# strip out all the generated directories and the "*.o" files from the source
# release
@allfiles = grep(!/^.*\.o$/ & !/^packaging\/SGI\/bins/ & !/^packaging\/SGI\/catman/ & !/^packaging\/SGI\/html/ & !/^packaging\/SGI\/codepages/, @allfiles);

open(IDB,">samba.idb") || die "Unable to open samba.idb for output\n";

print IDB "f 0644 root sys etc/config/samba packaging/SGI/samba.config samba.sw.base config(update)\n";
print IDB "f 0755 root sys etc/init.d/samba packaging/SGI/samba.rc samba.sw.base\n";
print IDB "l 0000 root sys etc/rc0.d/K39samba packaging/SGI samba.sw.base symval(../init.d/samba)\n";
print IDB "l 0000 root sys etc/rc2.d/S81samba packaging/SGI samba.sw.base symval(../init.d/samba)\n";

@copyfile = grep (/^COPY/,@allfiles);
print IDB "d 0755 root sys usr/relnotes/samba/ packaging/SGI samba.man.relnotes\n";
print IDB "f 0644 root sys usr/relnotes/samba/@copyfile[0] @copyfile[0] samba.man.relnotes\n";
print IDB "f 0644 root sys usr/relnotes/samba/legal_notice.html packaging/SGI/legal_notice.html samba.man.relnotes\n";
print IDB "f 0644 root sys usr/relnotes/samba/samba-relnotes.html packaging/SGI/relnotes.html samba.man.relnotes\n";

print IDB "d 0755 root sys usr/samba/ packaging/SGI samba.sw.base\n";
print IDB "f 0444 root sys usr/samba/README packaging/SGI/README samba.sw.base\n";

print IDB "d 0755 root sys usr/samba/bin/ packaging/SGI samba.sw.base\n";
while(@bins) {
  $nextfile = shift @bins;

  if (index($nextfile,'$')) {
    if ($nextfile eq "smbpasswd") {
      print IDB "f 0755 root sys usr/samba/bin/$nextfile source/$nextfile samba.sw.base\n";
    }
    elsif ($nextfile eq "findsmb") {
      print IDB "f 0755 root sys usr/samba/bin/$nextfile packaging/SGI/$nextfile samba.sw.base\n";
    }
    elsif ($nextfile eq "sambalp") {
      print IDB "f 0755 root sys usr/samba/bin/$nextfile packaging/SGI/$nextfile samba.sw.base\n";
    }
    elsif ($nextfile eq "smbprint") {
      print IDB "f 0755 root sys usr/samba/bin/$nextfile packaging/SGI/$nextfile samba.sw.base\n";
    }
    else {
      print IDB "f 0755 root sys usr/samba/bin/$nextfile source/$nextfile samba.sw.base\n";
    }
  }
}

print IDB "d 0755 root sys usr/samba/docs/ docs samba.man.doc\n";
while (@docs) {
  $nextfile = shift @docs;
  next if ($nextfile eq "CVS");
  ($junk,$file) = split(/\//,$nextfile,2);
  if (grep(/\/$/,$nextfile)) {
    print IDB "d 0755 root sys usr/samba/docs/$file $nextfile samba.man.doc\n";
  }
  else {
    print IDB "f 0644 root sys usr/samba/docs/$file $nextfile samba.man.doc\n";
  }
}

print IDB "f 0755 root sys usr/samba/inetd.sh packaging/SGI/inetd.sh samba.sw.base\n";
print IDB "d 0755 root sys usr/samba/lib/ packaging/SGI samba.sw.base\n";
if (@codepage) {
  print IDB "d 0755 root sys usr/samba/lib/codepages packaging/SGI samba.sw.base\n";
  while (@codepage) {
    $nextpage = shift @codepage;
    print IDB "f 0644 root sys usr/samba/lib/codepages/codepage.$nextpage packaging/SGI/codepages/codepage.$nextpage samba.sw.base\n";
  }
}
print IDB "f 0644 root sys usr/samba/lib/smb.conf packaging/SGI/smb.conf samba.sw.base config(update)\n";
print IDB "f 0755 root sys usr/samba/mkprintcap.sh packaging/SGI/mkprintcap.sh samba.sw.base\n";

print IDB "d 0644 root sys usr/samba/private/ packaging/SGI samba.sw.base\n";
print IDB "f 0600 root sys usr/samba/private/smbpasswd packaging/SGI/smbpasswd samba.sw.base config(update)\n";
print IDB "d 0755 root sys usr/samba/src/ packaging/SGI samba.src.samba\n";
@sorted = sort(@allfiles);
while (@sorted) {
  $nextfile = shift @sorted;
  ($file = $nextfile) =~ s/^.*\///;
  next if grep(/packaging\/SGI/& (/Makefile/ | /samba\.spec/ | /samba\.idb/),$nextfile);
  next if grep(/source/,$nextfile) && ($ignores{$file});
  next if ($nextfile eq "CVS");
  if (grep(/\/$/,$nextfile)) {
    print IDB "d 0755 root sys usr/samba/src/$nextfile $nextfile samba.src.samba\n";
  }
  else {
    if (grep((/\.sh$/ | /\.pl$/ | /mkman$/),$nextfile)) {
	print IDB "f 0755 root sys usr/samba/src/$nextfile $nextfile samba.src.samba\n";
    }
    else {
        print IDB "f 0644 root sys usr/samba/src/$nextfile $nextfile samba.src.samba\n";
    }
  }
}

print IDB "d 0755 root sys usr/samba/var/ packaging/SGI samba.sw.base\n";
print IDB "d 0755 root sys usr/samba/var/locks/ packaging/SGI samba.sw.base\n";

print IDB "d 0755 root sys usr/share/catman/u_man/ packaging/SGI samba.man.manpages\n";
$olddirnum = "0";
while (@catman) {
  $nextfile = shift @catman;
  ($file = $nextfile) =~ s/^packaging\/SGI\/catman\///;
  ($dirnum = $file) =~ s/^[\D]*//;
  $dirnum =~ s/\.Z//;
  if ($dirnum ne $olddirnum) {
    print IDB "d 0755 root sys usr/share/catman/u_man/cat$dirnum packaging/SGI samba.man.manpages\n";
    $olddirnum = $dirnum;
  }
  print IDB "f 0664 root sys usr/share/catman/u_man/cat$dirnum/$file $nextfile samba.man.manpages\n";
}

close IDB;
print "\n\nsamba.idb file has been created\n";

sub dodir {
    local($dir, $nlink) = @_;
    local($dev,$ino,$mode,$subcount);

    ($dev,$ino,$mode,$nlink) = stat('.') unless $nlink;

    opendir(DIR,'.') || die "Can't open $dir";
    local(@filenames) = sort readdir(DIR);
    closedir(DIR);

    if ($nlink ==2) {		# This dir has no subdirectories.
	for (@filenames) {
	    next if $_ eq '.';
	    next if $_ eq '..';
	    $this =  substr($dir,2)."/$_";
	    push(@allfiles,$this);
	}
    }
    else {
	$subcount = $nlink -2;
	for (@filenames) {
	    next if $_ eq '.';
	    next if $_ eq '..';
	    next if $_ eq 'CVS';
	    ($dev,$ino,$mode,$nlink) = lstat($_);
	    $name = "$dir/$_";
	    $this = substr($name,2);
	    $this .= '/' if -d;
	    push(@allfiles,$this);
	    next if $subcount == 0;		# seen all the subdirs?

	    next unless -d _;

	    chdir $_ || die "Can't cd to $name";
	    &dodir($name,$nlink);
	    chdir '..';
	    --$subcount;
	}
    }
}

sub byfilename {
  ($f0,$f1) = split(/\//,$a,2);
  ($f0,$f2) = split(/\//,$b,2);
  $f1 cmp $f2;
}

sub bydirnum {
  ($f1 = $a) =~ s/^.*\///;
  ($f2 = $b) =~ s/^.*\///;
  ($dir1 = $a) =~ s/^[\D]*//;
  ($dir2 = $b) =~ s/^[\D]*//;
  if (!($dir1 <=> $dir2)) {
    $f1 cmp $f2;
  }
  else {
    $dir1 <=> $dir2;
  }
}
