#ifndef USERNAMECARD_H
#define USERNAMECARD_H
#include <string>
#include <memory>
#include <cstdlib>

enum class Sex { Male, Female };

/*record the critical info of the user*/
struct UserNameCard {
  UserNameCard(const std::string& uuid, const std::string &avator_path, const std::string& nickname,
            const std::string& desc, Sex sex)
      : m_sex(sex), m_uuid(uuid), m_nickname(nickname), m_description(desc),
        m_avatorPath(avator_path) {}

  Sex m_sex;
  std::string m_uuid;
  std::string m_avatorPath;
  std::string m_nickname;
  std::string m_description;
};

#endif // USERNAMECARD_H
