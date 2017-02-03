package main

import "github.com/desertbit/photon"

func init() {
	go photon.Run()
}

func main() {
	<-make(chan struct{})
}
