package test

import (
	"fmt"
	"image"
	"image/color"
	"image/png"
	"log"
	"math"
	"os"
	"path"
	"path/filepath"
	"strconv"
	"strings"

	"code.google.com/p/graphics-go/graphics"

	"mistower.com/module/margs"
	"mistower.com/module/mfile"
	"mistower.com/module/mimage"
	"mistower.com/module/mjson"
	"mistower.com/module/mtype"
)

const (
	ITEM  = 0
	WALL  = 1
	FLOOR = 2
)

var (
	ATTENUATOR = 256
	DZeroNum   = 0
)

type Zb struct {
	X, Y int
}

type ShadowImg struct {
	Items image.Image
	Light Zb
	Wall  int
	Mode  string
}

var (
	funcMap []func(s *ShadowImg, l, itNum, Dis int) int
)

func init() {
	funcMap = []func(s *ShadowImg, l, itNum, Dis int) int{
		(*ShadowImg).cOfItem,
		(*ShadowImg).cOfWall,
		(*ShadowImg).cOfFloor,
	}
}

func (s *ShadowImg) Alpha(x, y int) uint8 {
	var (
		a uint8
	)
	_, _, _, shadow := s.At(x, y).RGBA()
	a = uint8(shadow >> 8)
	return a
}

func (s *ShadowImg) getI(k float32, j int) int {
	var (
		iFloat float32
	)
	iFloat = k*float32(j-s.Light.Y) + float32(s.Light.X)
	//fmt.Print(iFloat, " ")
	return int(iFloat)
}

func (s *ShadowImg) getJ(k float32, i int) int {
	var (
		jFloat float32
	)
	jFloat = float32(i-s.Light.X)/k + float32(s.Light.Y)
	return int(jFloat)
}

func (s *ShadowImg) cOfItem(l int, itNum int, Dis int) int {
	shadow := math.Min(float64(itNum)/float64(Dis+itNum)*255.0, 255.0)
	tmp := int(shadow)
	return tmp
}

func (s *ShadowImg) cOfWall(l int, itNum int, Dis int) int {
	//shadow := math.Min(float64(l), 128.0)
	//tmp := int(shadow)
	return 0
}

func (s *ShadowImg) cOfFloor(l int, itNum int, Dis int) int {
	var (
		shadow float64
	)
	if itNum-Dis > 0 {
		if Dis < 2 {
			Dis = 2
		}
		shadow = (1.0 - float64(Dis)/float64(itNum))
	} else {
		shadow = 0.0
	}

	tmp := int(shadow * 255.0)
	return tmp
}

func maxInt(a, b int) int {
	if a > b {
		return a
	}
	return b
}

func minInt(a, b int) int {
	if a < b {
		return a
	}
	return b
}

func ShowDZero() {
	fmt.Println(DZeroNum)
}

func (s *ShadowImg) getColorWithTp(l int, itNum int, Dis int, tp int) color.Color {
	var (
		r, g, b, a uint8
	)
	shadow := funcMap[tp](s, l, itNum, Dis)

	if s.Mode == "shadow" {
		if shadow > 0 {
			DZeroNum++
		}

		r = 0
		g = 0
		b = 0
		shadow = maxInt(shadow*2-255, 0)
		shadow = shadow + (l / 2)
		shadow = maxInt(shadow, 0)
		shadow = minInt(shadow, 255)
		//shadow = (shadow * shadow) >> 8
		a = uint8(shadow)

	} else {
		shadow = 255 - shadow
		shadow = maxInt(shadow*2-255, 0)
		shadow = shadow * (ATTENUATOR - l) / ATTENUATOR
		shadow = maxInt(shadow, 0)
		a = uint8(shadow) //uint8((shadow * shadow) >> 8)

		r = a
		g = a
		b = a - a/13
		//a = 255
	}

	return color.RGBA{
		r, g, b, a,
	}
}

func (s *ShadowImg) Bounds() image.Rectangle {
	return s.Items.Bounds()

}

func (s *ShadowImg) ColorModel() color.Model {
	return color.RGBAModel
}

func (s *ShadowImg) At(x, y int) color.Color {
	var (
		tp           int
		l            int
		k            float32
		itNum        int
		Dis          int
		withX, withY int
	)
	//_, _, _, a := s.Items.At(x, y).RGBA()

	if s.IsItem(x, y) {
		tp = ITEM
	} else {
		if y >= s.Wall {
			tp = FLOOR
		} else {
			tp = WALL
		}
	}

	l = (x-s.Light.X)*(x-s.Light.X) + (y-s.Light.Y)*(y-s.Light.Y)
	l = int(math.Sqrt(float64(l)))
	//lFloat := math.Sqrt(float64(l))
	fm := (y - s.Light.Y)

	minh := 5

	if fm >= -minh && fm <= minh {
		if fm < 0 {
			y = s.Light.Y - (minh-fm)/2 - 1
		} else {
			y = s.Light.Y + (minh+fm)/2 + 1
		}
		fm = y - s.Light.Y
	}

	k = ((float32(x - s.Light.X)) / float32(fm))

	if math.Abs(float64(k)) < 1.0 {

		//lFloat = lFloat / math.Sqrt(float64(k*k)+1.0)
		//l = int(lFloat)

		withX++

		if y > s.Light.Y {
			start := s.Light.Y
			end := y
			if s.Light.Y < 0 {
				start = 0
			}

			for j := start; j <= end; j++ {
				i := s.getI(k, j)
				if i >= 0 && i < s.Bounds().Dx() {
					//_, _, _, tmpA := s.Items.At(i, j).RGBA()
					if s.IsItem(i, j) {
						itNum++
						Dis = int(math.Abs(float64(j - y)))
					}
				}
			}

			if tp == ITEM {
				start = end
				end = s.Bounds().Dy()
				Dis = 0
				for j := start; j <= end; j++ {
					i := s.getI(k, j)
					if i >= 0 && i < s.Bounds().Dx() {
						//_, _, _, tmpA := s.Items.At(i, j).RGBA()
						if s.IsItem(i, j) {
							Dis++
						} else {
							break
						}
					} else {
						break
					}

				}
			}

		} else {

			start := s.Light.Y
			end := y
			if s.Light.Y > s.Items.Bounds().Dy() {
				start = s.Items.Bounds().Dy()
			}
			for j := start; j >= end; j-- {
				i := s.getI(k, j)
				//_, _, _, tmpA := s.Items.At(i, j).RGBA()
				if s.IsItem(i, j) {
					itNum++
					Dis = int(math.Abs(float64(j - y)))
				}
			}

			if tp == ITEM {
				end = 0
				start = y
				Dis = 0
				for j := start; j >= end; j-- {
					i := s.getI(k, j)
					if i >= 0 && i < s.Bounds().Dx() {
						//_, _, _, tmpA := s.Items.At(i, j).RGBA()
						if s.IsItem(i, j) {
							Dis++
						} else {
							break
						}
					} else {
						break
					}

				}
			}
		}

		itNum = int(math.Sqrt(float64(float32(itNum)*float32(itNum)*k*k + float32(itNum*itNum))))
		Dis = int(math.Sqrt(float64(float32(Dis)*float32(Dis)*k*k + float32(Dis*Dis))))
	} else {
		//k = float32(y-s.Light.Y)) / ((float32(x - s.Light.X))
		//lFloat = lFloat / math.Sqrt(float64(1.0/k/k)+1.0)
		//l = int(lFloat)

		withY++
		if x > s.Light.X {
			start := s.Light.X
			end := x
			if s.Light.X < 0 {
				start = 0
			}
			for i := start; i <= end; i++ {
				j := s.getJ(k, i)
				if s.IsItem(i, j) {
					itNum++
					Dis = int(math.Abs(float64(i - x)))
				}
			}

			if tp == ITEM {
				start = end
				end = s.Items.Bounds().Dx()
				Dis = 0
				for i := start; i <= end; i++ {
					j := s.getJ(k, i)
					if j >= 0 && j < s.Bounds().Dy() {
						if s.IsItem(i, j) {
							Dis++
						} else {
							break
						}
					} else {
						break
					}
				}
			}
		} else {
			start := s.Light.X
			end := x
			if s.Light.X > s.Items.Bounds().Dx() {
				start = s.Items.Bounds().Dx()
			}

			for i := start; i >= end; i-- {
				j := s.getJ(k, i)
				if s.IsItem(i, j) {
					itNum++
					Dis = int(math.Abs(float64(i - x)))
				}
			}

			if tp == ITEM {
				end = 0
				start = x
				Dis = 0
				for i := start; i >= end; i-- {
					j := s.getJ(k, i)
					if j >= 0 && j < s.Bounds().Dy() {
						if s.IsItem(i, j) {
							Dis++
						} else {
							break
						}
					} else {
						break
					}
				}
			}

		}

		itNum = int(math.Sqrt(float64(float32(itNum)*float32(itNum)/k/k + float32(itNum*itNum))))
		Dis = int(math.Sqrt(float64(float32(Dis)*float32(Dis)/k/k + float32(Dis*Dis))))

	}

	//tmp := math.Sqrt(float64(float32(itNum)*float32(itNum)*k*k + float32(itNum*itNum)))
	//tmp2 := math.Sqrt(float64(float32(Dis)*float32(Dis)*k*k + float32(Dis*Dis)))

	return s.getColorWithTp(l, itNum, Dis, tp)

}

func (s *ShadowImg) IsWall(i, j int) bool {
	if j < s.Wall {
		return s.IsItem(i, j)
	}
	return false
}

func (s *ShadowImg) getA(i, j int) int {
	if i >= 0 && i < s.Items.Bounds().Dx() && j >= 0 && j < s.Items.Bounds().Dy() {
		tmpR, tmpG, tmpB, tmpA := s.Items.At(i, j).RGBA()
		if tmpA >= (1 << 8) {
			if (tmpR>>8)+(tmpG>>8)+(tmpB>>8) > (250*3) && j < s.Wall {
				//fmt.Println("(", i, j, ")")
				return 0
			} else if tmpR == 0 && (tmpG == 0) && tmpB == 0 && tmpA < (250<<8) {
				return 0
			}
			return 1
		}
		return 0
	}
	return 0

}

func (s *ShadowImg) IsItem(x, y int) bool {
	if s.getA(x, y) == 1 {
		return true
	}
	return false
}

func processExt(filename string) string {
	var filenameWithSuffix string
	filenameWithSuffix = path.Base(filename)
	var fileSuffix string
	fileSuffix = path.Ext(filenameWithSuffix)
	var filenameOnly string
	filenameOnly = strings.TrimSuffix(filename, fileSuffix)
	return filenameOnly
}

func ProcessShadow(fileName string, lightX, lightY, wall int) {
	item, _ := mimage.LoadImage(fileName)
	shadow := ShadowImg{
		Items: item,
		Light: Zb{lightX, lightY},
		Wall:  wall,
		Mode:  "shadow",
	}

	fileNameTmp := processExt(fileName)

	w, err := os.Create(fileNameTmp + "_shadow.png")
	if err != nil {
		return
	}
	defer w.Close()
	png.Encode(w, &shadow)

	shadow.Mode = "light"
	w2, err := os.Create(fileNameTmp + "_light.png")
	if err != nil {
		return
	}
	defer w2.Close()
	png.Encode(w2, &shadow)

	return
}

type cr struct {
	Shadow uint8
	Set    bool
}

type blendAll struct {
	shadowList []ShadowImg
	Mode       string
	src        *([]cr)
	Padding    image.Image
	Ori        image.Image
}

func (b *blendAll) Bounds() image.Rectangle {
	return b.shadowList[0].Bounds()
}

func (b *blendAll) IsItem(x, y int) bool {
	for _, val := range b.shadowList {
		if val.IsItem(x, y) {
			return true
		}
	}
	return false
}

func (b *blendAll) IsWall(x, y int) bool {
	var num int
	for _, val := range b.shadowList {
		if val.IsWall(x, y) {
			num++
		}
	}
	if num == 1 {
		return true
	}
	return false
}

func (b *blendAll) isWindow(x, y int) bool {
	var num int
	if y >= b.shadowList[0].Wall {
		return false
	}
	for _, val := range b.shadowList {
		if val.IsItem(x, y) {
			num++
		}
	}
	if num == 0 {
		return true
	}
	return false
}

func maxUint8(a, b uint8) uint8 {
	if a > b {
		return a
	}
	return b
}

func minUint8(a, b uint8) uint8 {
	if a > b {
		return b
	}
	return a
}

func (s *ShadowImg) IsBlack(x, y int) bool {
	r, g, b, _ := s.Items.At(x, y).RGBA()
	return (r == 0 && g == 0 && b == 0)
}

func (b *blendAll) isPadding(x, y int) bool {
	if b.Padding != nil {
		_, _, _, a := b.Padding.At(x, y).RGBA()
		res := (a > (200 << 8))
		return res
	}
	return false
}

func (this *blendAll) blendOri(x, y int, li uint8) uint8 {
	if this.Ori != nil {
		r, g, b, _ := this.Ori.At(x, y).RGBA()
		r, g, b = r>>8, g>>8, b>>8
		y := (r*3 + g*6 + b*1) / 10
		res := (y*3 + 255) * (uint32(li))
		res = res >> 10
		return uint8(res)
	}
	return li
}

func (b *blendAll) At(x, y int) color.Color {
	var (
		res uint8
		//tmp int
	)

	if b.src == nil {
		src := make([]cr, b.Bounds().Dx()*b.Bounds().Dy())
		b.src = &src
		fmt.Println("create buffer")
	}

	sett := (*(b.src))[y*b.Bounds().Dx()+x]
	//num := len(b.shadowList)

	if sett.Set {
		res = sett.Shadow
	} else {
		if !b.IsItem(x, y) {
			if b.Mode == "light" {
				res = 255
			}
			for _, value := range b.shadowList {
				if b.Mode == "shadow" {
					res = maxUint8(value.Alpha(x, y), res)
				} else {
					res = minUint8(value.Alpha(x, y), res)
				}
			}

		} else {
			for _, value := range b.shadowList {
				if value.IsItem(x, y) {
					res = value.Alpha(x, y)
				}
			}
		}

		if b.IsWall(x, y) {
			if b.Mode == "shadow" {
				res = res >> 2
			} else {
				res = 0
			}
		}
		if b.isWindow(x, y) {
			if b.Mode == "shadow" {
				res = 0
			} else {
				res = 255
			}
		}

		if b.isPadding(x, y) {
			res = 0
		}

		if b.Mode == "light" {
			res = b.blendOri(x, y, res)
		}
		(*(b.src))[y*b.Bounds().Dx()+x].Set = true
		(*(b.src))[y*b.Bounds().Dx()+x].Shadow = res
	}

	if b.Mode == "shadow" {
		return color.RGBA{
			0, 0, 0, res,
		}
	} else {
		return color.RGBA{
			res, res, maxUint8(0, res-25), res,
		}
	}

}

func (b *blendAll) ColorModel() color.Model {
	return b.shadowList[0].ColorModel()
}

func (b *blendAll) SetMode(mode string) {
	b.Mode = mode
	b.src = nil
	for key, _ := range b.shadowList {
		b.shadowList[key].Mode = mode
	}
}

func Scale(src image.Image, bl int) image.Image {
	bound := src.Bounds()
	dx := bound.Dx()
	dy := bound.Dy()

	dst := image.NewRGBA(image.Rect(0, 0, dx*bl, dy*bl))
	err := graphics.Scale(dst, src)
	if err != nil {
		return nil
	}
	return dst
}

func GetConf() mtype.IM {
	var (
		CONF = "config.json"
	)
	if mfile.Exist(CONF) {
		data := make(mtype.IM)
		mjson.ReadJsonFile(CONF, &data)
		return data
	}
	return nil
}

func BlendTow() {
	var (
		//fileLists  = make([]string, 0, 2)
		shadowList = make([]ShadowImg, 0, 10)
	)
	args := margs.OsArgs()
	lightX, _ := strconv.Atoi((*args)["lx"])
	lightY, _ := strconv.Atoi((*args)["ly"])
	wall, _ := strconv.Atoi((*args)["wall"])
	padding := (*args)["pd"]
	conf := GetConf()
	rootDir := conf["root"].(string)
	ATTENUATOR = int(conf["atten"].(float64))

	fmt.Println("RootDir:", rootDir, ";attenu:", ATTENUATOR)

	bl, _ := strconv.Atoi((*args)["scale"])
	if (*args)["scale"] == "" {
		bl = 1
	}

	fileName := (*args)["fn"]
	fileName = filepath.Join(rootDir, fileName)
	originFileName := (*args)["ori"]

	for _, value := range os.Args {
		if strings.Contains(value, ".png") && (!strings.Contains(value, "=")) {
			//if len(shadowList) < 2 {
			//append(fileLists, value)
			item, _ := mimage.LoadImage(value)
			tmp := ShadowImg{
				Items: item,
				Light: Zb{lightX, lightY},
				Wall:  wall,
				Mode:  "shadow",
			}
			shadowList = append(shadowList, tmp)
			//} else {
			//	break
			//}
		}
	}

	bd := blendAll{
		shadowList: shadowList,
		Mode:       "shadow",
	}

	if padding != "" {
		bd.Padding, _ = mimage.LoadImage(padding)
	}

	if originFileName != "" {
		bd.Ori, _ = mimage.LoadImage(originFileName)
	}

	//fileNameTmp := processExt(fileName)

	newbd := Scale(&bd, bl)

	fmt.Println("Creating:", fileName+"_shadow.png")

	w, err := os.Create(fileName + "_shadow.png")
	if err != nil {
		log.Fatal(err)
		return
	}
	defer w.Close()
	png.Encode(w, newbd)

	bd.SetMode("light")

	newbd = Scale(&bd, bl)

	fmt.Println("Creating:", fileName+"_light.png")

	w2, err := os.Create(fileName + "_light.png")
	if err != nil {
		return
	}
	defer w2.Close()
	png.Encode(w2, newbd)

	return
}
