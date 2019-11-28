#!/usr/bin/env perl

use strict;

sub input {
    return <>;
}
sub getchar {
    my $ret = input();
    chomp($ret);
    return $ret;
}

sub help {
    for (;;) {
        print("Help? [y/n]");
        my $key = getchar();
        if ($key eq "y") {
            system("bash man.sh");
            last;
        } elsif ($key eq "n") {
            last;
        }
    }
}

sub getip {
    my $all = `ifconfig`;
    my $beg = index($all, "inet addr:192.168.");
    my $end = index($all, "Bcast");
    $beg = $beg + length("inet addr:");
    my $size = $end - $beg;
    if ($beg == -1 or $end == -1) {
        return "";
    }
    return substr($all, $beg, $size);
}

help();
system("clear");
my $ip = getip();
if ($ip eq "") {
    print("You are not connected to a local network.\n");
    die();
}
print("Port: ");
my $port = input();
my $cmd = "sudo ../server/tcpserv $ip $port";
system($cmd);
