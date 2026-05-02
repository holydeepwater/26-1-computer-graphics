# HW1_S20241620_안서현

HW1 Polygon Editor README (20241620 안서현)

1.	기본 기능 구현
a) 
initialize_polygon_editor() 사용해서 초기화
프로그램 시작 시 current_mode = MODE_IDLE로 설정
다각형 개수 n_polygons와 선택 상태 selected_polygon_index 초기화

b) 
관련 함수: start_create_mode(), add_point(), draw_open_lines_by_points(), finish_create_mode()
1: keyboard()에서 s 입력시 start_create_mode() 호출 – 현재 다각형 초기화, 색상 설정(set_polygon_color_by_index), 모드를 MODE_CREATE로 변경
2: mousepress()에서 SHIFT + Left Click 시 add_point() 호출
pg->point[i][0] = normalized_x;
pg->point[i][1] = normalized_y;
3: display()에서 draw_open_lines_by_points()로 현재 점들을 선으로 연결해 표시함
4: e 입력 시 finish_create_mode() – 점개수 3이상이어야 다각형 확정, update_center_of_gravity() 호출해서 중심 계산, 배열 polygons[]에 저장 후 MODE_IDLE로 복귀

c) 
관련 함수: update_center_of_gravity(), draw_center_point()
Center_x = sum(point[i][0])/n_points, center_y = sum(point[i][1])/n_points 로 계산 함. 모든 꼭짓점 평균 좌표로 계산하고, display()에서 항상 중심점 렌더링함

d) 
관련 함수: set_polygon_color_by_index()
다각형 순서에 따라서 색상 테이블에서 선택함. ( pg->line_color[0] = polygon_color_table[index][0]; )

e) 
관련 함수: find_polygon_by_center_click(), is_point_near_center()
MODE_IDLE에서 중심점 클릭 시 해당 다각형 index를 반환하고, selected_polygon_index를 설정. 이때 모드는 MODE_SELECT

f) 
관련 함수: mousepress()
MODE_SELECT 에서 중심점 클릭 후 놓아주기 selected_polygon_index = -1로 설정, MODE_IDLE로 전환함.

g)
관련 함수: remove_polygon_at()
C 입력 시 호출되고, 선택된 다각형 배열 후 배열 정렬. 
For (I = index; i<n-1; i++) polygons[i] = polygons[i+1] 로 삭제된 것 배열에서 없애기

h) 
관련함수: mousemove(), move_points()
delx = (x - prev_x) / width, dely = (prev_y - y) / height 로 모든 꼭짓점에 동일한 이동 적용함. 
중심점도 point[i] += (delx, dely) 로 같이 이동하게 함.

i)
관련 함수: rotate_points_around_center_of_grivity()_
delx = (x - prev_x) / width, dely = (prev_y - y) / height 로 중심 기준 회전, 마우스 이동량 비례해 각도 결정을 구현

j)
관련 함수: scale_points_around_center_of_grivity()
Point = scale * (point-center)+center 로 휠 방향에 따라 확대/축소 구현

k)
관련 함수: timer()
일정 시간 간격마다 호출 되고, 회전과 스케일 을 반복해서 적용함. 

l) 
관련 함수: set_background_color_by_mode()
모드에 따라서 glClearColor()을 변경함. Display()호출시 반영함.

2.	데이터 구조 및 핵심 구현
-	다각형 구조체 
typedef struct {
    float point[MAX_POSITIONS][2];
    int n_points;
    float center_x, center_y;
    float line_color[3];
} My_Polygon;

-	무게 중심 계산
center_x = sum(x_i) / n, center_y = sum(y_i) / n

-	상태 관리
EditorMode: 현재 모드 관리
Status: 마우스 상태 관리
pressed_vertex_index: 클릭된 꼭짓점
dragged_vertex_index: 드래그 여부 판단

3.	추가 기능 구현
1)	꼭짓점 삭제 및 이동
꼭짓점을 클릭하고 바로 놓으면  해당 꼭짓점 삭제
꼭짓점을 클릭 후 드래그하면  해당 꼭짓점 이동
이동 후 무게 중심을 다시 계산하여 반영
pressed_vertex_index, dragged_vertex_index 변수로 상태 관리
mouse down / move / up 이벤트를 분리하여 처리
이동 시 update_center_of_gravity() 호출
2)	꼭짓점 추가 (ctrl+leftclick)
Select Mode에서 Ctrl + Left Click (빈 공간) → 꼭짓점 추가
추가된 점을 포함하여 도형 재구성
무게 중심 자동 재계산
glutGetModifiers()로 Ctrl 입력 감지
insert_vertex() 함수로 좌표 변환 후 추가
3)	색상 변경 기능
Select Mode에서 f 키 입력
콘솔에서 RGB 값 (0~255) 입력
선택된 다각형의 색상 변경
입력값을 0~1 범위로 변환하여 line_color에 저장




5. 조작 방법 요약
기능	조작
생성 모드	s
점 추가	SHIFT + Left Click
생성 완료	e
선택	중심점 클릭
이동	Left Drag
회전	Right Drag
스케일	Mouse Wheel
삭제	c
애니메이션	a
색상 변경	f
꼭짓점 추가	Ctrl + Left Click
꼭짓점 삭제	클릭 후 release
꼭짓점 이동	클릭 후 drag
종료	q


