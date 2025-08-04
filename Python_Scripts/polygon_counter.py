import sys
import os
import trimesh

def get_polygon_count(file_path):
    """
    trimesh 라이브러리를 사용하여 FBX 파일의 총 폴리곤(면) 개수를 계산합니다.

    Args:
        file_path (str): 대상 FBX 파일의 경로.

    Returns:
        int: 파일 내 모든 메쉬의 총 폴리곤 개수.
    """
    try:
        # trimesh.load 함수는 파일에 여러 객체가 있으면 Scene 객체로,
        # 하나만 있으면 Trimesh 객체로 불러옵니다.
        # force='scene' 인자를 주면 항상 Scene 객체로 불러와 처리가 일관됩니다.
        scene = trimesh.load(file_path, force='scene')
        
        total_poly_count = 0
        
        # scene.geometry는 씬에 포함된 모든 메쉬 객체들을 담고 있습니다.
        # 이 객체들을 순회하며 각 메쉬의 면(faces) 개수를 더합니다.
        for geometry in scene.geometry.values():
            # geometry.faces는 폴리곤(면)의 배열이며, len()으로 개수를 셀 수 있습니다.
            total_poly_count += len(geometry.faces)
            
        return total_poly_count

    except FileNotFoundError:
        print(f"오류: 파일을 찾을 수 없습니다 -> {file_path}")
        return -1
    except Exception as e:
        print(f"파일을 읽는 중 오류가 발생했습니다: {e}")
        print("Trimesh가 FBX 파일을 처리하는데 필요한 의존성 라이브러리가 누락되었을 수 있습니다.")
        print("터미널에서 'pip install trimesh[easy]' 명령어를 실행해 보세요.")
        return -1

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("사용법: python <스크립트_이름>.py <FBX_파일_경로>")
        sys.exit(1)

    fbx_file_path = sys.argv[1]

    if not os.path.exists(fbx_file_path):
        print(f"오류: '{fbx_file_path}' 경로에 파일이 존재하지 않습니다.")
        sys.exit(1)
    
    print("폴리곤 개수를 계산하는 중입니다...")
    poly_count = get_polygon_count(fbx_file_path)

    if poly_count is not None and poly_count != -1:
        file_name = os.path.basename(fbx_file_path)
        print(f"\n파일 '{file_name}'의 총 폴리곤 개수: {poly_count}")
